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

#include "Scene/SceneObject.hpp"
#include "Scene/SceneObjectRegistrar.hpp"
#include "Scene/ARoutine.hpp"
#include "Scene/ARenderable.hpp"
#include "Math/Matrix4x4.hpp"
#include "Utilities/StringComposer.hpp"

#include "OcularEngine.hpp"

OCULAR_REGISTER_SCENEOBJECT(Ocular::Core::SceneObject, "SceneObject");

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Core
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        SceneObject::SceneObject(std::string const& name, SceneObject* parent, std::string const& type)
            : Object(name, type),
              m_IsStatic(false),
              m_IsActive(true),
              m_IsVisible(false),
              m_ForcedVisible(false),
              m_Persists(false),
              m_Renderable(nullptr),
              m_Parent(nullptr),
              m_Layer(0)
        {
            OcularScene->addObject(this, parent);
            
            OCULAR_EXPOSE(m_IsStatic);
            OCULAR_EXPOSE(m_ForcedVisible);
            OCULAR_EXPOSE(m_Transform);
        }

        SceneObject::SceneObject()
            : Object("SceneObject", "SceneObject"),
              m_IsStatic(false),
              m_IsActive(true),
              m_IsVisible(false),
              m_ForcedVisible(false),
              m_Persists(false),
              m_Renderable(nullptr),
              m_Parent(nullptr),
              m_Layer(0)
        {
            OcularScene->addObject(this);

            OCULAR_EXPOSE(m_IsStatic);
            OCULAR_EXPOSE(m_ForcedVisible);
            OCULAR_EXPOSE(m_Transform);
        }

        SceneObject::~SceneObject()
        {
            /** 
             * At this point the SceneManager/Scene have done the following:
             *
             *     - Removed this object from it's parent
             *     - Removed this object from the SceneTrees
             *     - Removed this object's routines from the Scene
             *     - Removed this object from the SceneManager
             *
             * What is left is to:
             *
             *     - Verify the above as much as possible
             *     - Destroy the routines
             *     - Destroy the renderables
             */

            //------------------------------------------------------------
            // Verify parent and children were removed

            if(m_Parent)
            {
                // Parent was not properly undone
                setParent(nullptr);
            }

            if(m_Children.size() > 0)
            {
                // Children were not properly destroyed
                while(m_Children.size() > 0)
                {
                    // This also calls child->setParent(nullptr) so removes it from our child list
                    OcularScene->destroyObject(m_Children[0]);
                }
            }

            //------------------------------------------------------------
            // Delete the routines

            while(m_Routines.size() > 0)
            {
                ARoutine* routine = m_Routines[0];

                if(routine)
                {
                    routine->onDestruction();
                    
                    delete routine;
                    routine = nullptr;
                }

                m_Routines.erase(m_Routines.begin());
            }

            //------------------------------------------------------------
            // Delete the renderable

            if(m_Renderable)
            {
                delete m_Renderable;
                m_Renderable = nullptr;
            }
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------
        // General Misc Methods
        //----------------------------------------------------------------

        void SceneObject::onVariableModified(std::string const& varName)
        {
            if(Utils::String::IsEqual(varName, "m_Transform"))
            {
                updateBounds(m_Transform.getDirtyFlags());
            }
        }

        Math::Transform& SceneObject::getTransform()
        {
            return m_Transform;
        }

        void SceneObject::setActive(bool active)
        {
            if(m_IsActive != active)
            {
                if(m_Parent)
                {
                    // Enforcing a strict cascading of activeness.
                    // If the parent is inactive, can not set a child as active.

                    if(active && !m_Parent->isActive())
                    {
                        return;
                    }
                }

                m_IsActive = active;
                OcularScene->objectActiveChanged(this);

                //--------------------------------------------------------
                // Propagate the state change to the child objects

                for(auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
                {
                    SceneObject* object = (*iter);

                    if(object)
                    {
                        object->setActive(active);
                    }
                }
            }
        }

        bool SceneObject::isActive() const
        {
            return m_IsActive;
        }

        void SceneObject::setVisible(bool visible)
        {
            m_IsVisible = visible;
        }

        bool SceneObject::isVisible() const
        {
            return m_IsVisible;
        }

        void SceneObject::setForcedVisible(bool forced)
        {
            if(m_ForcedVisible != forced)
            {
                m_ForcedVisible = forced;
                OcularScene->objectVisibleChanged(this);

                //--------------------------------------------------------
                // Propagate the state change to the child objects

                for(auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
                {
                    SceneObject* object = (*iter);

                    if(object)
                    {
                        object->setForcedVisible(forced);
                    }
                }
            }
        }

        bool SceneObject::isForcedVisible() const
        {
            return m_ForcedVisible;
        }

        void SceneObject::setStatic(bool isStatic)
        {
            if(m_IsStatic != isStatic)
            {
                m_IsStatic = isStatic;
                OcularScene->objectStaticChanged(this);
            }
        }

        bool SceneObject::isStatic() const
        {
            return m_IsStatic;
        }

        void SceneObject::setPersistent(bool persists)
        {
            m_Persists = persists;
        }

        bool SceneObject::isPersistent() const
        {
            return m_Persists;
        }

        Graphics::UniformPerObject const& SceneObject::getUniformData(Math::Matrix4x4 const& viewMatrix, Math::Matrix4x4 const& projMatrix)
        {
            m_UniformData.modelMatrix = getModelMatrix(false);

            m_UniformData.modelViewMatrix     = viewMatrix * m_UniformData.modelMatrix;
            m_UniformData.modelViewProjMatrix = projMatrix * m_UniformData.modelViewMatrix;
            m_UniformData.normalMatrix        = m_UniformData.modelMatrix.getInverse().getTranspose();

            return m_UniformData;
        }

        //----------------------------------------------------------------
        // Movement and Rotation Methods
        //----------------------------------------------------------------

        void SceneObject::setPosition(float const x, float const y, float const z)
        {
            m_Transform.setPosition(x, y, z);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::setPosition(Math::Vector3f const& position)
        {
            m_Transform.setPosition(position);
            updateBounds(m_Transform.getDirtyFlags());
        }

        Math::Vector3f SceneObject::getPosition(bool const local) const
        {
            Math::Vector4f position;

            if(local)
            {
                position = Math::Vector4f(m_Transform.getPosition());
            }
            else
            {
                const Math::Matrix4x4 matrix = getModelMatrix(false);
                position = matrix * position;
            }

            return Math::Vector3f(position.x, position.y, position.z);
        }

        void SceneObject::translate(Math::Vector3f const& translation, bool local)
        {
            m_Transform.translate(translation, local);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::moveForward(float const distance)
        {
            m_Transform.moveForward(distance);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::moveUp(float const distance)
        {
            m_Transform.moveUp(distance);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::moveRight(float const distance)
        {
            m_Transform.moveRight(distance);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::rotate(float const angle, Math::Vector3f const& axis)
        {
            m_Transform.rotate(angle, axis);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::rotate(Math::Quaternion const& rotation)
        {
            m_Transform.rotate(rotation);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::resetRotation()
        {
            m_Transform.setRotation(Math::Quaternion());
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::setRotation(Math::Quaternion const& rotation)
        {
            m_Transform.setRotation(rotation);
            updateBounds(m_Transform.getDirtyFlags());
        }

        Math::Quaternion const& SceneObject::getRotation() const
        {
            return m_Transform.getRotation();
        }

        void SceneObject::setScale(Math::Vector3f const& scale)
        {
            m_Transform.setScale(scale);
            updateBounds(m_Transform.getDirtyFlags());
        }

        void SceneObject::setScale(float const xScale, float const yScale, float const zScale)
        {
            m_Transform.setScale(Math::Vector3f(xScale, yScale, zScale));
            updateBounds(m_Transform.getDirtyFlags());
        }

        Math::Vector3f SceneObject::getScale(bool const local) const
        {
            Math::Vector3f result = m_Transform.getScale();

            if(!local)
            {
                if(m_Parent)
                {
                    result *= m_Parent->getScale(local);
                }
            }
            
            return result;
        }

        void SceneObject::setTransform(Math::Transform const& transform)
        {
            m_Transform = transform;

            updateBounds(static_cast<uint32_t>(Math::Transform::DirtyFlags::Position) | 
                         static_cast<uint32_t>(Math::Transform::DirtyFlags::Rotation) | 
                         static_cast<uint32_t>(Math::Transform::DirtyFlags::Scale));
        }

        void SceneObject::lookAt(Math::Vector3f const& point)
        {
            m_Transform.lookAt(point);
            updateBounds(m_Transform.getDirtyFlags());
        }

        Math::Transform const& SceneObject::getTransform() const
        {
            return m_Transform;
        }

        Math::Matrix4x4 SceneObject::getModelMatrix(bool const local) const
        {
            Math::Matrix4x4 result;
            const Math::Matrix4x4 localMatrix = m_Transform.getModelMatrix();

            if(local)
            {
                result = localMatrix;
            }
            else
            {
                Math::Matrix4x4 parentMatrix;

                if(m_Parent)
                {
                    parentMatrix = m_Parent->getModelMatrix(false);
                }

                result = parentMatrix * localMatrix;
            }

            return result;
        }

        //----------------------------------------------------------------
        // Child Object Methods
        //----------------------------------------------------------------

        void SceneObject::setParent(SceneObject* parent, bool maintainWorldPos)
        {
            if(parent != m_Parent)
            {
                if(parent)
                {
                    parent->addChild(this, maintainWorldPos);
                }
                else
                {
                    if(maintainWorldPos)
                    {
                        // Will be a top-level object. Just set position to our old world position.
                        setPosition(getPosition(false));
                    }

                    SceneObject* oldParent = m_Parent;

                    if(oldParent)
                    {
                        oldParent->removeChild(this);
                    }

                    m_Parent = parent;
                    forceBoundsRebuild();
                }
            }
        }

        SceneObject* SceneObject::getParent() const
        {
            return m_Parent;
        }

        SceneObject* SceneObject::createChild(std::string const& name)
        {
            SceneObject* child = OcularScene->createObject(name, this);
            return child;
        }

        void SceneObject::addChild(SceneObject* child, bool maintainWorldPos)
        {
            if(child && (child->m_Parent != this))
            {
                if(maintainWorldPos)
                {
                    // Set the child's new local position so that it is the same as it's old world position
                    const Math::Vector3f oldWorldPos = child->getPosition(false);
                    child->setPosition(oldWorldPos - getPosition(false));
                }

                SceneObject* oldParent = child->getParent();

                if(oldParent)
                {
                    oldParent->removeChild(child);
                }

                child->m_Parent = this;
                child->setActive(isActive());
                child->setForcedVisible(isForcedVisible());
                child->setStatic(isStatic());
                child->forceBoundsRebuild();

                m_Children.emplace_back(child);

                OcularScene->objectParentChanged(child, oldParent);
            }
        }

        SceneObject* SceneObject::findChild(std::string const& name)
        {
            SceneObject* result = nullptr;

            for(auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
            {
                SceneObject* child = (*iter);

                if(child)
                {
                    if(child->getName().compare(name) == 0)
                    {
                        result = child;
                        break;
                    }
                }
            }

            return result;
        }

        SceneObject* SceneObject::findChild(UUID const& uuid)
        {
            SceneObject* result = nullptr;

            for(auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
            {
                SceneObject* child = (*iter);

                if(child)
                {
                    if(child->getUUID() == uuid)
                    {
                        result = child;
                        break;
                    }
                }
            }

            return result;
        }

        SceneObject* SceneObject::removeChild(std::string const& name)
        {
            SceneObject* result = nullptr;

            for(auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
            {
                SceneObject* child = (*iter);

                if(child)
                {
                    if(child->getName().compare(name) == 0)
                    {
                        result = child;
                        m_Children.erase(iter);

                        child->m_Parent = nullptr;
                        OcularScene->objectParentChanged(child, this);

                        break;
                    }
                }
            }

            return result;
        }

        SceneObject* SceneObject::removeChild(UUID const& uuid)
        {
            SceneObject* result = nullptr;

            for(auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
            {
                SceneObject* child = (*iter);

                if(child)
                {
                    if(child->getUUID() == uuid)
                    {
                        result = child;
                        m_Children.erase(iter);

                        child->m_Parent = nullptr;
                        OcularScene->objectParentChanged(child, this);

                        break;
                    }
                }
            }

            return result;
        }

        SceneObject* SceneObject::removeChild(SceneObject const* object)
        {
            SceneObject* result = nullptr;

            for(auto iter = m_Children.begin(); iter != m_Children.end(); ++iter)
            {
                SceneObject* child = (*iter);

                if(child == object)
                {
                    result = child;
                    m_Children.erase(iter);

                    child->m_Parent = nullptr;
                    OcularScene->objectParentChanged(child, this);

                    break;
                }
            }

            return result;
        }

        uint32_t SceneObject::getNumChildren() const
        {
            return static_cast<uint32_t>(m_Children.size());
        }

        std::vector<SceneObject*> const& SceneObject::getAllChildren() const
        {
            return m_Children;
        }

        //----------------------------------------------------------------
        // Routine Methods
        //----------------------------------------------------------------

        ARoutine* SceneObject::addRoutine(std::string const& name)
        {
            ARoutine* result = OcularScene->getRoutineFactory().createComponent(name);

            if(result)
            {
                m_Routines.push_back(result);

                result->setParent(this);
                result->setName(name);
                result->onCreation();
            }

            return result;
        }

        void SceneObject::addRoutine(ARoutine* routine)
        {
            if(routine)
            {
                m_Routines.push_back(routine);
                routine->setParent(this);
            }
        }

        bool SceneObject::removeRoutine(std::string const& name)
        {
            bool result = false;

            for(auto iter = m_Routines.begin(); iter != m_Routines.end(); ++iter)
            {
                ARoutine* routine = (*iter);

                if(routine)
                {
                    if(routine->getName().compare(name) == 0)
                    {
                        m_Routines.erase(iter);

                        OcularScene->objectRemovedRoutine(routine);
                        routine->onDestruction();

                        delete routine;
                        routine = nullptr;

                        result = true;
                        break;
                    }
                }
            }

            return result;
        }

        bool SceneObject::removeRoutine(ARoutine* routine, bool transferring)
        {
            bool result = false;

            if(routine)
            {
                for(auto iter = m_Routines.begin(); iter != m_Routines.end(); ++iter)
                {
                    ARoutine* iterRoutine = (*iter);

                    if(iterRoutine == routine)
                    {
                        m_Routines.erase(iter);

                        if(!transferring)
                        {
                            OcularScene->objectRemovedRoutine(routine);
                            routine->onDestruction();

                            delete routine;
                            routine = nullptr;
                        }

                        result = true;
                        break;
                    }
                }
            }

            return result;
        }

        void SceneObject::removeAllRoutines()
        {
            while(m_Routines.size() > 0)
            {
                ARoutine* routine = (*m_Routines.begin());
                m_Routines.erase(m_Routines.begin());

                if(routine)
                {
                    OcularScene->objectRemovedRoutine(routine);
                    routine->onDestruction();

                    delete routine;
                    routine = nullptr;
                }
            }
        }

        ARoutine* SceneObject::getRoutine(std::string const& name)
        {
            ARoutine* result = nullptr;

            for(auto iter = m_Routines.begin(); iter != m_Routines.end(); ++iter)
            {
                ARoutine* routine = (*iter);

                if(routine)
                {
                    if(routine->getName().compare(name) == 0)
                    {
                        result = routine;
                        break;
                    }
                }
            }

            return result;
        }

        std::vector<ARoutine*> const& SceneObject::getAllRoutines() const
        {
            return m_Routines;
        }

        uint32_t SceneObject::getNumRoutines() const
        {
            return static_cast<uint32_t>(m_Routines.size());
        }

        //----------------------------------------------------------------
        // Renderable Methods
        //----------------------------------------------------------------

        ARenderable* SceneObject::setRenderable(std::string const& name)
        {
            ARenderable* result = OcularScene->getRenderableFactory().createComponent(name);

            if(result)
            {
                removeRenderable();

                result->m_Parent = this;

                m_Renderable = result;
                m_Renderable->initialize();
            }

            return result;
        }

        void SceneObject::setRenderable(ARenderable* renderable)
        {
            removeRenderable();

            if(renderable)
            {
                SceneObject* oldParent = renderable->getParent();

                if(oldParent)
                {
                    oldParent->removeRenderable(true);
                }

                renderable->m_Parent = this;
                m_Renderable = renderable;

                // Assume here that the renderable has already been initialized
            }
        }

        void SceneObject::removeRenderable(bool transferring)
        {
            if(m_Renderable)
            {
                if(!transferring)
                {
                    delete m_Renderable;
                }
                
                m_Renderable = nullptr;
            }
        }

        ARenderable* SceneObject::getRenderable() const
        {
            return m_Renderable;
        }

        //----------------------------------------------------------------
        // Inherited Methods
        //----------------------------------------------------------------

        void SceneObject::onLoad(BuilderNode const* node)
        {
            const UUID old = m_UUID;

            Object::onLoad(node);

            if(old != m_UUID)
            {
                /*
                 * Work around.
                 * 
                 * When we load a SceneObject, the SceneObject has already added itself to
                 * the SceneManager under a generated UUID before this method is even called.
                 * But in this method (or rather in the super call), our UUID may be changed.
                 * 
                 * We need to alert the SceneManager of this UUID change.
                 */

                OcularScene->updateUUID(old);
            }

            if(node)
            {
                //----------------------------------------------------
                // Load Renderable
                //----------------------------------------------------

                const BuilderNode* renderableNode = node->getChild("m_Renderable");

                if(renderableNode)
                {
                    setRenderable(renderableNode->getType());

                    if(m_Renderable)
                    {
                        m_Renderable->onLoad(renderableNode);
                    }
                }

                //----------------------------------------------------
                // Load Routines
                //----------------------------------------------------

                std::vector<BuilderNode*> routineNodes;
                node->findChildrenByName(routineNodes, "Routine_");

                for(auto routineNode : routineNodes)
                {
                    if(routineNode)
                    {
                        ARoutine* routine = addRoutine(routineNode->getType());

                        if(routine)
                        {
                            routine->onLoad(routineNode);
                        }
                    }
                }

                //----------------------------------------------------
                // Load Child Objects
                //----------------------------------------------------

                // Tentative as it will only work on pure SceneObjects currently...

                std::vector<BuilderNode*> childNodes;
                node->findChildrenByName(childNodes, "Child_");

                for(auto childNode : childNodes)
                {
                    if(childNode)
                    {
                        SceneObject* child = OcularScene->createObjectOfType(childNode->getType(), "", this);

                        if(child)
                        {
                            addChild(child);
                            child->onLoad(childNode);
                        }
                        else
                        {
                            OcularLogger->error("Failed to create child object of type '", childNode->getType(), "'", OCULAR_INTERNAL_LOG("SceneObject", "onLoad"));
                        }
                    }
                }
            }
        }

        void SceneObject::onSave(BuilderNode* node) const
        {
            if(!m_Persists)
            {
                Object::onSave(node);

                if(node)
                {
                    //----------------------------------------------------
                    // Save Renderable
                    //----------------------------------------------------

                    if(m_Renderable)
                    {
                        BuilderNode* renderableNode = node->addChild("m_Renderable", m_Renderable->getClass(), "");

                        if(renderableNode)
                        {
                            m_Renderable->onSave(renderableNode);
                        }
                    }

                    //----------------------------------------------------
                    // Save Routines
                    //----------------------------------------------------

                    const uint32_t numRoutines = static_cast<uint32_t>(m_Routines.size());

                    if(numRoutines > 0)
                    {
                        for(uint32_t i = 0; i < numRoutines; i++)
                        {
                            ARoutine* routine = m_Routines[i];

                            if(routine)
                            {
                                BuilderNode* routineNode = node->addChild(OCULAR_STRING_COMPOSER("Routine_", i), routine->getClass(), "");
                                routine->onSave(routineNode);
                            }
                        }
                    }

                    //----------------------------------------------------
                    // Save Child Objects
                    //----------------------------------------------------

                    // Tentative as it will only work on pure SceneObjects currently...

                    const uint32_t numChildren = static_cast<uint32_t>(m_Children.size());

                    if(numChildren > 0)
                    {
                        for(uint32_t i = 0; i < numChildren; i++)
                        {
                            SceneObject* child = m_Children[i];

                            if(child)
                            {
                                BuilderNode* childNode = node->addChild(OCULAR_STRING_COMPOSER("Child_", i), child->getClass(), "");
                                child->onSave(childNode);
                            }
                        }
                    }
                }
            }
        }

        //----------------------------------------------------------------
        // Bounds Related
        //----------------------------------------------------------------

        void SceneObject::forceBoundsRebuild()
        {
            if(m_Renderable)
            {
                m_Renderable->buildBounds(&m_BoundsSphereLocal, &m_BoundsAABBLocal, &m_BoundsOBBLocal);
            }

            updateBounds(static_cast<uint32_t>(Math::Transform::DirtyFlags::Position) | 
                         static_cast<uint32_t>(Math::Transform::DirtyFlags::Rotation) | 
                         static_cast<uint32_t>(Math::Transform::DirtyFlags::Scale));
        }

        void SceneObject::updateBounds(uint32_t const dirtyFlags)
        {
            if(dirtyFlags)
            {
                const Math::Matrix4x4 modelMatrix = getModelMatrix(false); 

                bool boundsUpdated = false;

                if(dirtyFlags & static_cast<uint32_t>(Math::Transform::DirtyFlags::Rotation))
                {
                    if(m_Renderable)
                    {
                        m_Renderable->buildBounds(nullptr, &m_BoundsAABBWorld, nullptr, modelMatrix);
                        boundsUpdated = true;
                    }
                }
                else if(dirtyFlags & static_cast<uint32_t>(Math::Transform::DirtyFlags::Scale))
                {
                    // If scale is dirty we must adjust both the size and position of the bounds.
                    // Note that position is only affected if its a cascading scale change (ie coming
                    // from the parent), but we perform the adjustment everytime for simplicity.

                    const Math::Vector3f scale = getScale();
                    const float maxScale = fmaxf(fmaxf(scale.x, scale.y), scale.z);
                    
                    m_BoundsSphereWorld.setCenter((modelMatrix * m_BoundsSphereLocal.getCenter()));
                    m_BoundsSphereWorld.setRadius(m_BoundsSphereLocal.getRadius() * maxScale);

                    m_BoundsAABBWorld.setCenter((modelMatrix * m_BoundsAABBLocal.getCenter()));
                    m_BoundsAABBWorld.setExtents(m_BoundsAABBLocal.getExtents() * maxScale);

                    m_BoundsOBBWorld.setCenter((modelMatrix * m_BoundsOBBWorld.getCenter()));
                    m_BoundsOBBWorld.setExtents(m_BoundsOBBLocal.getExtents() * scale);

                    boundsUpdated = true;
                }
                else if(dirtyFlags & static_cast<uint32_t>(Math::Transform::DirtyFlags::Position))
                {
                    m_BoundsSphereWorld.setCenter((modelMatrix * m_BoundsSphereLocal.getCenter()));
                    m_BoundsAABBWorld.setCenter((modelMatrix * m_BoundsAABBLocal.getCenter()));
                    m_BoundsOBBWorld.setCenter((modelMatrix * m_BoundsOBBWorld.getCenter()));

                    boundsUpdated = true;
                }

                if(boundsUpdated)
                {
                    OcularScene->triggerObjectDirty(m_UUID, m_IsStatic);
                }

                for(auto child : m_Children)
                {
                    child->updateBounds(dirtyFlags);
                }
            }
        }

        Math::BoundsSphere SceneObject::getBoundsSphere(bool const local)
        {
            Math::BoundsSphere result = m_BoundsSphereLocal;

            if(!local)
            {
                updateBounds(m_Transform.getDirtyFlags());
                result = m_BoundsSphereWorld;
            }

            return result;
        }

        Math::BoundsAABB SceneObject::getBoundsAABB(bool const local)
        {
            Math::BoundsAABB result = m_BoundsAABBLocal;

            if(!local)
            {
                updateBounds(m_Transform.getDirtyFlags());
                result = m_BoundsAABBWorld;
            }

            return result;
        }

        Math::BoundsOBB SceneObject::getBoundsOBB(bool const local)
        {
            Math::BoundsOBB result = m_BoundsOBBLocal;

            if(!local)
            {
                updateBounds(m_Transform.getDirtyFlags());
                result = m_BoundsOBBWorld;
            }

            return result;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        void SceneObject::getModelMatrix(Math::Matrix4x4& matrix)
        {
            Math::Matrix4x4 parentMatrix;

            if(m_Parent)
            {
                m_Parent->getModelMatrix(parentMatrix);
            }

            matrix = parentMatrix * m_Transform.getModelMatrix();
        }

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}