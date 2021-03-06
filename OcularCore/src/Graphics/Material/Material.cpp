/**
 * Copyright 2014-2017 Steven T Sell (ssell@vertexfragment.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Graphics/Material/Material.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "Graphics/Shader/ShaderProgram.hpp"

#include "Math/Matrix3x3.hpp"
#include "Utilities/StringComposer.hpp"

#include "OcularEngine.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Graphics
    {
        const std::string Material::ShaderNodeName      = "ShaderProgram";
        const std::string Material::TexturesNodeName    = "Textures";
        const std::string Material::TextureNodeName     = "Texture";
        const std::string Material::UniformsNodeName    = "Uniforms";
        const std::string Material::UniformNodeName     = "Uniform";
        const std::string Material::RenderStateNodeName = "RenderState";

        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        Material::Material()
            : Core::Resource(),
              m_VertexShader(nullptr),
              m_GeometryShader(nullptr),
              m_FragmentShader(nullptr),
              m_PreTessellationShader(nullptr),
              m_PostTessellationShader(nullptr),
              m_RenderPriority(static_cast<uint32_t>(Core::RenderPriority::Opaque))
        {
            m_Type = Core::ResourceType::Material;
            m_UniformBuffer = OcularGraphics->createUniformBuffer(UniformBufferType::PerMaterial);
            m_Textures.reserve(OcularGraphics->getMaxBoundTextures());

            RenderState* renderState = OcularGraphics->getRenderState();

            if(renderState)
            {
                m_StoredRasterState = renderState->getRasterState();
                m_StoredRasterState.primitiveStyle = PrimitiveStyle::TriangleList;
                m_StoredRasterState.fillMode = FillMode::Solid;
            }

            setDefaults();
        }

        Material::~Material()
        {
            unbind();

            if(m_UniformBuffer)
            {
                delete m_UniformBuffer;
                m_UniformBuffer = nullptr;
            }
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        void Material::bind()
        {
            bindStateChanges();
            bindShaders();
            //bindTextures();

            m_UniformBuffer->bind();
        }

        void Material::unbind()
        {
            unbindStateChanges();
            unbindShaders();
        }

        void Material::unload()
        {
            // Intentionally left empty
        }

        void Material::onLoad(Core::BuilderNode const* node)
        {
            ObjectIO::onLoad(node);

            if(node)
            {
                onLoadShaders(node);
                onLoadTextures(node);
                onLoadUniforms(node);
                onLoadRenderState(node);
            }
        }

        void Material::onSave(Core::BuilderNode* node) const
        {
            ObjectIO::onSave(node);

            if(node)
            {
                onSaveShaders(node);
                onSaveTextures(node);
                onSaveUniforms(node);
                onSaveRenderState(node);
            }
        }

        //----------------------------------------------------------------------------------
        // Texture Methods
        //----------------------------------------------------------------------------------

        bool Material::setTexture(uint32_t const index, std::string const& name, Texture* texture)
        {
            bool result = false;

            if(index < OcularGraphics->getMaxBoundTextures())
            {
                bool registerInUse = false;

                for(auto iter = m_Textures.begin(); iter != m_Textures.end(); ++iter)
                {
                    if((*iter).samplerRegister == index)
                    {
                        (*iter).samplerName = name;
                        (*iter).texture = texture;

                        registerInUse = true;
                    }
                }

                if(!registerInUse)
                {
                    TextureSamplerInfo sampler;
                    sampler.texture = texture;
                    sampler.samplerName = name;
                    sampler.samplerRegister = index;

                    m_Textures.emplace_back(sampler);
                }

                result = true;
            }
            else
            {
                OcularLogger->warning("Specified Texture register index of ", index, " exceeds maximum register index of ", (OcularGraphics->getMaxBoundTextures() - 1), OCULAR_INTERNAL_LOG("Material", "setTexture"));
            }

            return result;
        }

        Texture* Material::getTexture(uint32_t const index) const
        {
            Texture* result = nullptr;

            if(index < OcularGraphics->getMaxBoundTextures())
            {
                for(auto iter = m_Textures.begin(); iter != m_Textures.end(); ++iter)
                {
                    if((*iter).samplerRegister == index)
                    {
                        result = (*iter).texture;
                    }
                }
            }
            else
            {
                OcularLogger->warning("Specified Texture register index of ", index, " exceeds maximum register index of ", (OcularGraphics->getMaxBoundTextures() - 1), OCULAR_INTERNAL_LOG("Material", "getTexture"));
            }

            return result;
        }

        void Material::removeTexture(uint32_t const index)
        {
            if(index < OcularGraphics->getMaxBoundTextures())
            {
                for(auto iter = m_Textures.begin(); iter != m_Textures.end(); ++iter)
                {
                    if((*iter).samplerRegister == index)
                    {
                        m_Textures.erase(iter);
                    }
                }
            }
            else
            {
                OcularLogger->warning("Specified Texture register index of ", index, " exceeds maximum register index of ", (OcularGraphics->getMaxBoundTextures() - 1), OCULAR_INTERNAL_LOG("Material", "removeTexture"));
            }
        }

        uint32_t Material::getNumTextures() const
        {
            return static_cast<uint32_t>(m_Textures.size());
        }

        std::vector<TextureSamplerInfo> const* Material::getTextures() const
        {
            return &m_Textures;
        }

        //----------------------------------------------------------------------------------
        // Shader Methods
        //----------------------------------------------------------------------------------

        bool Material::setVertexShader(std::string const& name)
        {
            bool result = false;
            ShaderProgram* program = OcularResources->getResource<ShaderProgram>(name);

            if(program)
            {
                m_VertexShader = program->getVertexShader();

                if(m_VertexShader)
                {
                    result = true;
                }
                else
                {
                    OcularLogger->warning("Found matching ShaderProgram '", name, "' but it did not contain a vertex shader", OCULAR_INTERNAL_LOG("Material", "setVertexShader"));
                }
            }
            else
            {
                OcularLogger->warning("No ShaderProgram was found with the name '", name, "'", OCULAR_INTERNAL_LOG("Material", "setVertexShader"));
            }

            return result;
        }

        void Material::setVertexShader(VertexShader* shader)
        {
            // Allow to set for null here to 'disable' the shader.
            m_VertexShader = shader;
        }

        bool Material::setGeometryShader(std::string const& name)
        {
            bool result = false;
            ShaderProgram* program = OcularResources->getResource<ShaderProgram>(name);

            if(program)
            {
                m_GeometryShader = program->getGeometryShader();

                if(m_GeometryShader)
                {
                    result = true;
                }
                else
                {
                    OcularLogger->warning("Found matching ShaderProgram '", name, "' but it did not contain a geometry shader", OCULAR_INTERNAL_LOG("Material", "setGeometryShader"));
                }
            }
            else
            {
                OcularLogger->warning("No ShaderProgram was found with the name '", name, "'", OCULAR_INTERNAL_LOG("Material", "setGeometryShader"));
            }

            return result;
        }

        void Material::setGeometryShader(GeometryShader* shader)
        {
            // Allow to set for null here to 'disable' the shader.
            m_GeometryShader = shader;
        }

        bool Material::setFragmentShader(std::string const& name)
        {
            bool result = false;
            ShaderProgram* program = OcularResources->getResource<ShaderProgram>(name);

            if(program)
            {
                m_FragmentShader = program->getFragmentShader();

                if(m_FragmentShader)
                {
                    result = true;
                }
                else
                {
                    OcularLogger->warning("Found matching ShaderProgram '", name, "' but it did not contain a fragment shader", OCULAR_INTERNAL_LOG("Material", "setFragmentShader"));
                }
            }
            else
            {
                OcularLogger->warning("No ShaderProgram was found with the name '", name, "'", OCULAR_INTERNAL_LOG("Material", "setFragmentShader"));
            }

            return result;
        }

        void Material::setFragmentShader(FragmentShader* shader)
        {
            // Allow to set for null here to 'disable' the shader.
            m_FragmentShader = shader;
        }

        bool Material::setPreTessellationShader(std::string const& name)
        {
            bool result = false;
            ShaderProgram* program = OcularResources->getResource<ShaderProgram>(name);

            if(program)
            {
                m_PreTessellationShader = program->getPreTessellationShader();

                if(m_PreTessellationShader)
                {
                    result = true;
                }
                else
                {
                    OcularLogger->warning("Found matching ShaderProgram '", name, "' but it did not contain a pre-tessellation shader", OCULAR_INTERNAL_LOG("Material", "setPreTessellationShader"));
                }
            }
            else
            {
                OcularLogger->warning("No ShaderProgram was found with the name '", name, "'", OCULAR_INTERNAL_LOG("Material", "setPreTessellationShader"));
            }

            return result;
        }

        void Material::setPreTessellationShader(PreTessellationShader* shader)
        {
            // Allow to set for null here to 'disable' the shader.
            m_PreTessellationShader = shader;
        }

        bool Material::setPostTessellationShader(std::string const& name)
        {
            bool result = false;
            ShaderProgram* program = OcularResources->getResource<ShaderProgram>(name);

            if(program)
            {
                m_PostTessellationShader = program->getPostTessellationShader();

                if(m_PostTessellationShader)
                {
                    result = true;
                }
                else
                {
                    OcularLogger->warning("Found matching ShaderProgram '", name, "' but it did not contain a post-tessellation shader", OCULAR_INTERNAL_LOG("Material", "setPostTessellationShader"));
                }
            }
            else
            {
                OcularLogger->warning("No ShaderProgram was found with the name '", name, "'", OCULAR_INTERNAL_LOG("Material", "setPostTessellationShader"));
            }

            return result;
        }

        void Material::setPostTessellationShader(PostTessellationShader* shader)
        {
            // Allow to set for null here to 'disable' the shader.
            m_PostTessellationShader = shader;
        }

        VertexShader* Material::getVertexShader() const
        {
            return m_VertexShader;
        }

        GeometryShader* Material::getGeometryShader() const
        {
            return m_GeometryShader;
        }

        FragmentShader* Material::getFragmentShader() const
        {
            return m_FragmentShader;
        }

        PreTessellationShader* Material::getPreTessellationShader() const
        {
            return m_PreTessellationShader;
        }

        PostTessellationShader* Material::getPostTessellationShader() const
        {
            return m_PostTessellationShader;
        }

        //----------------------------------------------------------------------------------
        // Uniform Methods
        //----------------------------------------------------------------------------------

        void Material::setUniform(std::string const& name, uint32_t registerIndex, float const value)
        {
            Uniform uniform;
            uniform.setName(name);
            uniform.setType(OCULAR_TYPE_NAME(float));
            uniform.setRegister(registerIndex);
            uniform.setData(value);

            m_UniformBuffer->setUniform(uniform);
        }

        bool Material::getUniform(std::string const& name, float& value)
        {
            bool result = false;
            Uniform const* uniform = m_UniformBuffer->getUniform(name);

            if(uniform)
            {
                if(uniform->getSize() == 1)
                {
                    value = uniform->getElement(0);
                    result = true;
                }
                else
                {
                    OcularLogger->error("Improper uniform request (requesting single float for non-single float uniform)", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                }
            }

            return result;
        }

        void Material::setUniform(std::string const& name, uint32_t registerIndex, Math::Vector4f const& value)
        {
            Uniform uniform;
            uniform.setName(name);
            uniform.setType(OCULAR_TYPE_NAME(Math::Vector4f));
            uniform.setRegister(registerIndex);
            uniform.setData(value);

            m_UniformBuffer->setUniform(uniform);
        }

        bool Material::getUniform(std::string const& name, Math::Vector4f& value)
        {
            bool result = false;
            Uniform const* uniform = m_UniformBuffer->getUniform(name);

            if(uniform)
            {
                if(uniform->getSize() == 4)
                {
                    float const* data = uniform->getData();

                    if(data)
                    {
                        value.x = data[0];
                        value.y = data[1];
                        value.z = data[2];
                        value.w = data[3];

                        result = true;
                    }
                    else
                    {
                        OcularLogger->error("Uniform data is NULL", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                    }
                }
                else
                {
                    OcularLogger->error("Improper uniform request (requesting vector for non-vector uniform)", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                }
            }

            return result;
        }

        void Material::setUniform(std::string const& name, uint32_t registerIndex, Core::Color const& value)
        {
            Uniform uniform;
            uniform.setName(name);
            uniform.setType(OCULAR_TYPE_NAME(Core::Color));
            uniform.setRegister(registerIndex);
            uniform.setData(value);

            m_UniformBuffer->setUniform(uniform);
        }

        bool Material::getUniform(std::string const& name, Core::Color& value)
        {
            bool result = false;
            Uniform const* uniform = m_UniformBuffer->getUniform(name);

            if(uniform)
            {
                if(uniform->getSize() == 4)
                {
                    float const* data = uniform->getData();

                    if(data)
                    {
                        value.r = data[0];
                        value.g = data[1];
                        value.b = data[2];
                        value.a = data[3];

                        result = true;
                    }
                    else
                    {
                        OcularLogger->error("Uniform data is NULL", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                    }
                }
                else
                {
                    OcularLogger->error("Improper uniform request (requesting vector for non-vector uniform)", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                }
            }

            return result;
        }

        void Material::setUniform(std::string const& name, uint32_t registerIndex, Math::Matrix3x3 const& value)
        {
            Uniform uniform;
            uniform.setName(name);
            uniform.setType(OCULAR_TYPE_NAME(Math::Matrix3x3));
            uniform.setRegister(registerIndex);
            uniform.setData(value);

            m_UniformBuffer->setUniform(uniform);
        }

        bool Material::getUniform(std::string const& name, Math::Matrix3x3& value)
        {
            bool result = false;
            Uniform const* uniform = m_UniformBuffer->getUniform(name);

            if(uniform)
            {
                if(uniform->getSize() == 12)
                {
                    float const* data = uniform->getData();

                    if(data)
                    {
                        value.setData(uniform->getData());
                        result = true;
                    }
                    else
                    {
                        OcularLogger->error("Uniform data is NULL", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                    }
                }
                else
                {
                    OcularLogger->error("Improper uniform request (requesting 3x3 matrix for non-3x3 matrix uniform)", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                }
            }

            return result;
        }

        void Material::setUniform(std::string const& name, uint32_t registerIndex, Math::Matrix4x4 const& value)
        {
            Uniform uniform;
            uniform.setName(name);
            uniform.setType(OCULAR_TYPE_NAME(Math::Matrix4x4));
            uniform.setRegister(registerIndex);
            uniform.setData(value);

            m_UniformBuffer->setUniform(uniform);
        }

        bool Material::getUniform(std::string const& name, Math::Matrix4x4& value)
        {
            bool result = false;
            Uniform const* uniform = m_UniformBuffer->getUniform(name);

            if(uniform)
            {
                if(uniform->getSize() == 16)
                {
                    float const* data = uniform->getData();

                    if(data)
                    {
                        value.setData(uniform->getData());
                        result = true;
                    }
                    else
                    {
                        OcularLogger->error("Uniform data is NULL", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                    }
                }
                else
                {
                    OcularLogger->error("Improper uniform request (requesting 3x3 matrix for non-4x4 matrix uniform)", OCULAR_INTERNAL_LOG("Material", "getUniform"));
                }
            }

            return result;
        }

        UniformBuffer const* Material::getUniformBuffer() const
        {
            return m_UniformBuffer;
        }

        //----------------------------------------------------------------------------------
        // Render State Changes
        //----------------------------------------------------------------------------------

        void Material::setPrimitiveStyle(PrimitiveStyle const style)
        {
            m_StoredRasterState.primitiveStyle = style;
        }

        PrimitiveStyle Material::getPrimitiveStyle() const
        {
            return m_StoredRasterState.primitiveStyle;
        }

        void Material::setFillMode(FillMode const mode)
        {
            m_StoredRasterState.fillMode = mode;
        }

        FillMode Material::getFillMode() const
        {
            return m_StoredRasterState.fillMode;
        }

        void Material::setRenderPriority(uint32_t const priority)
        {
            m_RenderPriority = priority;
        }

        uint32_t Material::getRenderPriority() const
        {
            return m_RenderPriority;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        void Material::setDefaults()
        {
            ShaderProgram* shaders = OcularResources->getResource<Graphics::ShaderProgram>("OcularCore/Shaders/Default");

            if(shaders)
            {
                m_VertexShader           = shaders->getVertexShader();
                m_GeometryShader         = shaders->getGeometryShader();
                m_FragmentShader         = shaders->getFragmentShader();
                m_PreTessellationShader  = shaders->getPreTessellationShader();
                m_PostTessellationShader = shaders->getPostTessellationShader();
            }
        }

        void Material::bindShaders()
        {
            if(m_VertexShader)
            {
                m_VertexShader->bind();
            }

            if(m_GeometryShader)
            {
                m_GeometryShader->bind();
            }

            if(m_FragmentShader)
            {
                m_FragmentShader->bind();
            }

            if(m_PreTessellationShader)
            {
                m_PreTessellationShader->bind();
            }

            if(m_PostTessellationShader)
            {
                m_PostTessellationShader->bind();
            }
        }

        void Material::unbindShaders()
        {
            if(m_VertexShader)
            {
                m_VertexShader->unbind();
            }

            if(m_GeometryShader)
            {
                m_GeometryShader->unbind();
            }

            if(m_FragmentShader)
            {
                m_FragmentShader->unbind();
            }

            if(m_PreTessellationShader)
            {
                m_PreTessellationShader->unbind();
            }

            if(m_PostTessellationShader)
            {
                m_PostTessellationShader->unbind();
            }
        }

        void Material::bindStateChanges()
        {
            RenderState* renderState = OcularGraphics->getRenderState();

            if(renderState)
            {
                bool modified = false;
                RasterState currState = renderState->getRasterState();

                if(m_StoredRasterState.primitiveStyle != currState.primitiveStyle)
                {
                    currState.primitiveStyle = m_StoredRasterState.primitiveStyle;
                    modified = true;
                }

                if(m_StoredRasterState.fillMode != currState.fillMode)
                {
                    currState.fillMode = m_StoredRasterState.fillMode;
                    modified = true;
                }

                if(modified)
                {
                    renderState->setRasterState(currState);
                    renderState->bind();
                }
            }
        }

        void Material::unbindStateChanges()
        {

        }

        void Material::onLoadShaders(Core::BuilderNode const* parent)
        {
            const Core::BuilderNode* shaderProgramNode = parent->getChild("ShaderProgram");

            if(shaderProgramNode)
            {
                const Core::BuilderNode* vertexNode = shaderProgramNode->getChild("Vertex");

                if(vertexNode)
                {
                    ShaderProgram* program = OcularResources->getResource<ShaderProgram>(vertexNode->getValue());

                    if(program)
                    {
                        m_VertexShader = program->getVertexShader();
                            
                        if(m_VertexShader)
                        {
                            m_VertexShader->setMappingName(vertexNode->getValue());
                        }
                    }
                }

                const Core::BuilderNode* geometryNode = shaderProgramNode->getChild("Geometry");

                if(geometryNode)
                {
                    ShaderProgram* program = OcularResources->getResource<ShaderProgram>(geometryNode->getValue());

                    if(program)
                    {
                        m_GeometryShader = program->getGeometryShader();
                            
                        if(m_GeometryShader)
                        {
                            m_GeometryShader->setMappingName(vertexNode->getValue());
                        }
                    }
                }

                const Core::BuilderNode* fragmentNode = shaderProgramNode->getChild("Fragment");

                if(fragmentNode)
                {
                    ShaderProgram* program = OcularResources->getResource<ShaderProgram>(fragmentNode->getValue());

                    if(program)
                    {
                        m_FragmentShader = program->getFragmentShader();
                            
                        if(m_FragmentShader)
                        {
                            m_FragmentShader->setMappingName(vertexNode->getValue());
                        }
                    }
                }

                const Core::BuilderNode* preTessellationNode = shaderProgramNode->getChild("PreTessellation");

                if(preTessellationNode)
                {
                    ShaderProgram* program = OcularResources->getResource<ShaderProgram>(preTessellationNode->getValue());

                    if(program)
                    {
                        m_PreTessellationShader = program->getPreTessellationShader();
                            
                        if(m_PreTessellationShader)
                        {
                            m_PreTessellationShader->setMappingName(vertexNode->getValue());
                        }
                    }
                }

                const Core::BuilderNode* postTessellationNode = shaderProgramNode->getChild("PostTessellation");

                if(postTessellationNode)
                {
                    ShaderProgram* program = OcularResources->getResource<ShaderProgram>(postTessellationNode->getValue());

                    if(program)
                    {
                        m_PostTessellationShader = program->getPostTessellationShader();
                            
                        if(m_PostTessellationShader)
                        {
                            m_PostTessellationShader->setMappingName(vertexNode->getValue());
                        }
                    }
                }
            }
        }

        void Material::onLoadTextures(Core::BuilderNode const* parent)
        {
            const Core::BuilderNode* texturesNode = parent->getChild(TexturesNodeName);

            if(texturesNode)
            {
                std::vector<Core::BuilderNode*> textureNodes;
                texturesNode->findChildrenByName(textureNodes, TextureNodeName);

                m_Textures.clear();
                m_Textures.reserve(textureNodes.size());

                for(auto textureNode : textureNodes)
                {
                    // Call setTexture instead of adding directly to the m_Textures container so
                    // that any API-specific implementations (aka D3D11Material) may work properly.

                    auto textureNameNode = textureNode->getChild("Name");
                    auto textureValueNode = textureNode->getChild("Value");
                    auto textureRegisterNode = textureNode->getChild("Register");

                    if(textureNameNode && textureValueNode && textureRegisterNode)
                    {
                        setTexture(
                            OcularString->fromString<uint32_t>(textureRegisterNode->getValue()),
                            textureNameNode->getValue(),
                            OcularResources->getResource<Texture>(textureValueNode->getValue()));
                    }
                }
            }
        }

        void Material::onLoadUniforms(Core::BuilderNode const* parent)
        {
            const Core::BuilderNode* uniformsNode = parent->getChild(UniformsNodeName);

            if(uniformsNode)
            {
                std::vector<Core::BuilderNode*> uniformChildren;
                uniformsNode->findChildrenByName(uniformChildren, UniformNodeName);

                for(auto uniformNode : uniformChildren)
                {
                    Uniform uniform;

                    onLoadUniform(uniformNode, &uniform);

                    m_UniformBuffer->setUniform(uniform);
                }
            }
        }

        void Material::onLoadUniform(Core::BuilderNode const* parent, Uniform* uniform)
        {
            if(parent && uniform)
            {
                const auto nameNode = parent->getChild("Name");

                if(nameNode)
                {
                    uniform->setName(nameNode->getValue());
                }

                const auto valueNode = parent->getChild("Value");

                if(valueNode)
                {
                    // Allow myself this ugly if/else chain as the types available
                    // as uniforms is very restricted and not subject to change.

                    const auto typeStr = valueNode->getType();

                    uniform->setType(typeStr);

                    if(Utils::String::IsEqual(typeStr, OCULAR_TYPE_NAME(float)))
                    {
                        float data = OcularString->fromString<float>(valueNode->getValue());
                        uniform->setData(data);
                    }
                    else if(Utils::String::IsEqual(typeStr, OCULAR_TYPE_NAME(Math::Vector4f)))
                    {
                        Math::Vector4f data = OcularString->fromString<Math::Vector4f>(valueNode->getValue());
                        uniform->setData(data);
                    }
                    else if(Utils::String::IsEqual(typeStr, OCULAR_TYPE_NAME(Core::Color)))
                    {
                        Core::Color data = OcularString->fromString<Core::Color>(valueNode->getValue());
                        uniform->setData(data);
                    }
                    else if(Utils::String::IsEqual(typeStr, OCULAR_TYPE_NAME(Math::Matrix3x3)))
                    {
                        Math::Matrix3x3 data = OcularString->fromString<Math::Matrix3x3>(valueNode->getValue());
                        uniform->setData(data);
                    }
                    else if(Utils::String::IsEqual(typeStr, OCULAR_TYPE_NAME(Math::Matrix4x4)))
                    {
                        Math::Matrix4x4 data = OcularString->fromString<Math::Matrix4x4>(valueNode->getValue());
                        uniform->setData(data);
                    }
                }

                const auto registerNode = parent->getChild("Register");

                if(registerNode)
                {
                    uniform->setRegister(OcularString->fromString<uint32_t>(registerNode->getValue()));
                }
            }
        }

        void Material::onLoadRenderState(Core::BuilderNode const* parent)
        {
            const Core::BuilderNode* renderStateNode = parent->getChild(RenderStateNodeName);

            if(renderStateNode)
            {
                const Core::BuilderNode* primitiveStyleNode = renderStateNode->getChild("PrimitiveStyle");

                if(primitiveStyleNode)
                {
                    m_StoredRasterState.primitiveStyle = static_cast<PrimitiveStyle>(OcularString->fromString<uint32_t>(primitiveStyleNode->getValue()));

                    if(m_StoredRasterState.primitiveStyle > PrimitiveStyle::Undefined)
                    {
                        m_StoredRasterState.primitiveStyle = PrimitiveStyle::TriangleList;
                    }
                }

                const Core::BuilderNode* fillModeNode = parent->getChild("FillMode");

                if(fillModeNode)
                {
                    m_StoredRasterState.fillMode = static_cast<FillMode>(OcularString->fromString<uint32_t>(fillModeNode->getValue()));

                    if(m_StoredRasterState.fillMode > FillMode::Undefined)
                    {
                        m_StoredRasterState.fillMode = FillMode::Solid;
                    }
                }
            }
        }

        void Material::onSaveShaders(Core::BuilderNode* parent) const
        {
            Core::BuilderNode* shaderProgramNode = parent->addChild(ShaderNodeName, "", "");

            if(shaderProgramNode)
            {
                if(m_VertexShader)
                {
                    shaderProgramNode->addChild("Vertex", "Shader", m_VertexShader->getMappingName());
                }
                    
                if(m_GeometryShader)
                {
                    shaderProgramNode->addChild("Geometry", "Shader", m_GeometryShader->getMappingName());
                }
                    
                if(m_FragmentShader)
                {
                    shaderProgramNode->addChild("Fragment", "Shader", m_FragmentShader->getMappingName());
                }
                    
                if(m_PreTessellationShader)
                {
                    shaderProgramNode->addChild("PreTessellation", "Shader", m_PreTessellationShader->getMappingName());
                }
                    
                if(m_PostTessellationShader)
                {
                    shaderProgramNode->addChild("PostTessellation", "Shader", m_PostTessellationShader->getMappingName());
                }
            }
        }

        void Material::onSaveTextures(Core::BuilderNode* parent) const
        {
            Core::BuilderNode* texturesNode = parent->addChild(TexturesNodeName, "", "");

            if(texturesNode)
            {
                uint32_t nodeIndex = 0;

                for(auto texture : m_Textures)
                {
                    if(texture.texture)
                    {
                        const auto textureNodeName = OCULAR_STRING_COMPOSER(Ocular::Graphics::Material::TextureNodeName, nodeIndex++);
                        auto textureNode = texturesNode->addChild(textureNodeName, "", "");

                        textureNode->addChild("Name", OCULAR_TYPE_NAME(std::string), texture.samplerName);
                        textureNode->addChild("Value", "Texture", texture.texture->getMappingName());
                        textureNode->addChild("Register", OCULAR_TYPE_NAME(uint32_t), OcularString->toString<uint32_t>(texture.samplerRegister));
                    }
                }
            }
        }

        void Material::onSaveUniforms(Core::BuilderNode* parent) const
        {
            Core::BuilderNode* uniformsNode = parent->addChild(UniformsNodeName, "", "");

            if(uniformsNode && m_UniformBuffer)
            {
                uint32_t nodeIndex = 0;

                for(uint32_t i = 0; i < m_UniformBuffer->getNumUniforms(); i++)
                {
                    const auto uniformNodeName = OCULAR_STRING_COMPOSER(Ocular::Graphics::Material::UniformNodeName, nodeIndex++);
                    auto uniformNode = uniformsNode->addChild(uniformNodeName, "", "");

                    onSaveUniform(uniformNode, m_UniformBuffer->getUniform(i));
                }
            }
        }

        void Material::onSaveUniform(Core::BuilderNode* parent, Uniform const* uniform) const
        {
            if(parent && uniform)
            {
                const auto size = uniform->getSize();

                const std::string nameStr     = uniform->getName();
                const std::string typeStr     = uniform->getType();
                const std::string registerStr = OcularString->toString<uint32_t>(uniform->getRegister());

                std::string valueStr = "";

                if(size)
                {
                    if(size == 1)
                    {
                        float value = uniform->getData()[0];
                        valueStr = OcularString->toString<float>(value);
                    }
                    else
                    {
                        const float* value = uniform->getData();
                        valueStr = OcularString->toString(typeStr, void_cast<const float*>(value));
                    }
                }

                parent->addChild("Name", OCULAR_TYPE_NAME(std::string), nameStr);
                parent->addChild("Value", typeStr, valueStr);
                parent->addChild("Register", OCULAR_TYPE_NAME(uint32_t), registerStr);
            }
        }

        void Material::onSaveRenderState(Core::BuilderNode* parent) const
        {

        }

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}