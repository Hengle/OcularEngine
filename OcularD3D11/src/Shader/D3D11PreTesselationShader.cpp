/**
 * Copyright 2014-2015 Steven T Sell (ssell@ocularinteractive.com)
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

#include "stdafx.hpp"
#include "Shader/D3D11PreTesselationShader.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Graphics
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        D3D11PreTesselationShader::D3D11PreTesselationShader(ID3D11DeviceContext* context)
            : PreTesselationShader(),
              m_D3DDeviceContext(context),
              m_D3DShader(nullptr),
              m_D3DBlob(nullptr)
        {
            m_Type = Core::ResourceType::Shader;
        }

        D3D11PreTesselationShader::~D3D11PreTesselationShader()
        {
            unload();
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------
        
        void D3D11PreTesselationShader::unload()
        {
            PreTesselationShader::unload();

            if(m_D3DShader)
            {
                m_D3DShader->Release();
                m_D3DShader = nullptr;
            }

            if(m_D3DBlob)
            {
                m_D3DBlob->Release();
                m_D3DBlob = nullptr;
            }
        }

        void D3D11PreTesselationShader::bind()
        {
            PreTesselationShader::bind();

            if(m_D3DDeviceContext)
            {
                m_D3DDeviceContext->HSSetShader(m_D3DShader, nullptr, 0);
            }
        }
            
        void D3D11PreTesselationShader::unbind()
        {
            PreTesselationShader::unbind();

            if(m_D3DDeviceContext)
            {
                m_D3DDeviceContext->HSSetShader(nullptr, nullptr, 0);
            }
        }

        void D3D11PreTesselationShader::setD3DShader(ID3D11HullShader* shader)
        {
            if(m_D3DShader)
            {
                m_D3DShader->Release();
            }

            m_D3DShader = shader;
        }

        ID3D11HullShader* D3D11PreTesselationShader::getD3DShader()
        {
            return m_D3DShader;
        }

        void D3D11PreTesselationShader::setD3DBlob(ID3DBlob* blob)
        {
            if(m_D3DBlob)
            {
                m_D3DBlob->Release();
            }

            m_D3DBlob = blob;
        }

        ID3DBlob* D3D11PreTesselationShader::getD3DBlob()
        {
            return m_D3DBlob;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}