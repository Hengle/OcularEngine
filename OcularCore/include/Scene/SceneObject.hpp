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

#pragma once
#ifndef __H__OCULAR_CORE_SCENE_OBJECT__H__
#define __H__OCULAR_CORE_SCENE_OBJECT__H__

#include "Object.hpp"

#include "ObjectIO/ObjectIO.hpp"

#include "Math/Transform.hpp"
#include "Math/Bounds/BoundsSphere.hpp"
#include "Math/Bounds/BoundsAABB.hpp"
#include "Math/Bounds/BoundsOBB.hpp"

#include "Graphics/Shader/Uniform/UniformPerObject.hpp"

#include <string>
#include <map>
#include <vector>

#include "Math/Matrix3x3.hpp"

//------------------------------------------------------------------------------------------

/**
 * \addtogroup Ocular
 * @{
 */
namespace Ocular
{
    namespace Math
    {
        class Matrix4x4;
    }

    /**
     * \addtogroup Core
     * @{
     */
    namespace Core
    {
        class SceneManager;
        class ARoutine;
        class ARenderable;

        /**
         * \class SceneObject
         *
         * A SceneObject represents the instance of an arbitrary object in a Scene.
         * This can range from a static scenery object, such as a rock, to a particle effect,
         * to a physics volume, to a multifaceted actor.
         *
         * A single SceneObject can have multiple child objects attached to it. This can be 
         * thought of as similar to a car. A parent chassis object could have child axle objects
         * which in turn have child wheel objects. Each level down inherits the parent's transform
         * and active/visible status (unless otherwise overridden).
         *
         * Additionally, there are Routine and Renderable instances that may be attached to
         * a SceneObject. A Routine represents the logic behind an object while the Renderable
         * is any part of the object that is to be rendered onto the screen.
         *
         * When a SceneObject is created, it is automatically added to the current SceneManager which
         * then takes ownership of the object. This means it will be automatically destroyed when
         * the current Scene is unloaded or when the SceneManager is destroyed in the case of an
         * object marked as persistent.
         *
         * There are two options when creating/destroying a standalone object:
         *
         *     SceneObject* object = new SceneObject();
         *     delete object;
         *
         *     SceneObject* object = OcularScene->createObject();
         *     OcularScene->destroyObject();
         *
         * In either case, the SceneManager owns the object and handles adding it to the Scene,
         * invoking the Routines and Renderables, intersection testing, etc.
         */
        class SceneObject : public Object
        {
            friend class SceneManager;

        public:
            
            /**
             * Creates a new SceneObject with the specified name.
             *
             * \note The object is automatically added to the Ocular SceneManager, and the SceneManager
             *       takes ownership of it. If a completely unmanaged object is required see ... (not yet available)
             *
             * \param[in] name   Identifier name of the object. Note that the name does not need to be unique as
             *                   all objects are already uniquely identified via a UUID (see getUUID()).
             * \param[in] parent 
             */
            SceneObject(std::string const& name, SceneObject* parent = nullptr, std::string const& type = "SceneObject");

            /**
             * Creates a new SceneObject with the default name of "SceneObject".
             *
             * \note The object is automatically added to the Ocular SceneManager, and the SceneManager
             *       takes ownership of it. If a completely unmanaged object is required see ... (not yet available)
             */
            SceneObject();

            virtual ~SceneObject();

            //------------------------------------------------------------
            // General Misc. Methods
            //------------------------------------------------------------

            /**
             * Reacts to the modification of certain exposed variables.
             * Variables that are reacted to include:
             *
             *     - m_Rotation
             *     - m_Scale
             */
            virtual void onVariableModified(std::string const& varName) override;

            /**
             * Returns the current local transform of this object.
             */
            Math::Transform& getTransform();

            /**
             * Sets whether this object is active or not. 
             * An inactive object will not have it's render or update methods called.
             *
             * The active state propagates to child SceneObjects.
             *
             * If attempting to set active, and the parent is currently inactive,
             * the state will not be changed. 
             *
             * Children may not be set active while the parent is inactive, 
             * but they can be set inactive while the parent is active.
             *
             * \param[in] active (default TRUE)
             */
            void setActive(bool active);

            /**
             * Returns if the object is active.
             */
            bool isActive() const;

            /**
             * Sets whether this object is visible or not. Note that this method
             * is called every frame by the scene, and thus manually calling it
             * will have little or no effect.
             *
             * If you wish to force an object to always be rendered, then you
             * should instead call setForcedVisible.
             *
             * \param[in] visible
             */
            void setVisible(bool visible);

            /**
             * \return If TRUE, then the object is being rendered.
             */
            bool isVisible() const;

            /**
             * Sets whether this object is forced visible. If an object is forced
             * visible, then it's renderables will always be rendered irregardless
             * of whether or not it is in view or passes any culling tests.
             *
             * \note This flag has a cascading effect. Any children of a forced visible
             *       object will automatically be forced visible themselves.
             *
             * \param[in] forced
             */
            void setForcedVisible(bool forced);

            /**
             * \return TRUE if the object is being forced visible and being rendered irregardless of culling tests.
             */
            bool isForcedVisible() const;

            /**
             * Sets whether this object is static or not. If an object is static,
             * then no transformations (translation, rotation, scaling) will have
             * any effect on it.
             *
             * This is useful for objects such as terrain pieces that once created,
             * will never move. By setting to static, it allows for numerous rendering
             * optimizations to take place.
             *
             * \note This flag has a cascading effect. Any children of a static object
             *       will automatically be static themselves.
             *
             * \param[in] isStatic
             */
            void setStatic(bool isStatic);

            /**
             * \return TRUE if the object (or a parent) is marked as static.
             */
            bool isStatic() const;

            /**
             * Sets whether this object should persist inbetween scenes.
             *
             * Typically, all active objects are unloaded along with the currently
             * active scene. However, if an object is marked as persistent, then
             * it will not be unloaded with the scene and will automatically added
             * to the new scene.
             *
             * \param[in] persists
             */
            void setPersistent(bool persists);

            /**
             * \return TRUE if this object persists between scenes.
             */
            bool isPersistent() const;

            /**
             *
             */
            Graphics::UniformPerObject const& getUniformData(Math::Matrix4x4 const& viewMatrix, Math::Matrix4x4 const& projMatrix);

            //------------------------------------------------------------
            // Movement and Rotation Methods
            //------------------------------------------------------------

            /**
             * Sets the position of the SceneObject.
             *
             * \param[in] x
             * \param[in] y
             * \param[in] z
             */
            void setPosition(float x, float y, float z);

            /**
             * Sets the position of the SceneObject.
             * \param[in]position
             */
            void setPosition(Math::Vector3f const& position);

            /**
             * \return The SceneObject's position
             */
            Math::Vector3f getPosition(bool local = true) const;

            /**
             * Translates the SceneObject along the given vector
             * \param[in] translation
             */
            void translate(Math::Vector3f const& translation, bool local = true);

            void moveForward(float distance);

            void moveUp(float distance);

            void moveRight(float distance);

            /**
             * Rotates the SceneObject along the given axis by the given angle
             *
             * \param[in] angle Angle to rotate in degrees
             * \param[in] axis  Axis to rotate around
             */
            void rotate(float angle, Math::Vector3f const& axis);

            /**
             *
             */
            void rotate(Math::Quaternion const& rotation);

            /**
             * Sets the SceneObject's rotation
             * \param[in] rotation
             */
            void setRotation(Math::Quaternion const& rotation);

            /**
             * Resets the orientation of the object
             */
            void resetRotation();

            /**
             *
             */
            Math::Quaternion const& getRotation() const;

            /**
             *
             */
            void setScale(Math::Vector3f const& scale);

            /**
             *
             */
            void setScale(float xScale, float yScale, float zScale);

            /**
             *
             */
            Math::Vector3f getScale(bool local = true) const;

            /**
             *
             */
            void setTransform(Math::Transform const& transform);

            /**
             *
             */
            void lookAt(Math::Vector3f const& point);

            /**
             *
             */
            Math::Transform const& getTransform() const;

            /**
             *
             */
            virtual Math::Matrix4x4 getModelMatrix(bool local = true) const;

            //------------------------------------------------------------
            // Child Object Methods
            //------------------------------------------------------------

            /**
             * Sets the specified SceneOject as the parent of this SceneObject.
             * 
             * If the new parent is not null, then the cascading states of the parent
             * (active, forced visible, static/dynamic, etc.) are applied to this object.
             *
             * May also specify whether or not to preserve the absolute world position.
             *
             * By default, the world position is maintained. This means that the local position
             * is modified such that, upon being added to the new parent, the world position 
             * remains the same. 
             *
             * Alternatively (by setting the maintainWorldPos parameter to FALSE), the local
             * position will remain the same but the absolute world position will be different.
             *
             * For example:
             *
             * We have a parentless SceneObject positioned at (0, 5, 0).
             * We assign it a parent at position (5, 0, 0).
             *
             * If maintainWorldPos is TRUE, then the SceneObject's absolute world position will
             * remain as (0, 5, 0), while its new local position to the parent is (-5, 5, 0)
             *
             * If maintainWorld is FALSE, then the SceneObject's local position to the parent 
             * will remain as (0, 5, 0), while it's new world position will be (5, 5, 0).
             *
             * \param[in] parent New parent of this object. Pass NULL if no parent is desired.
             * \param[in] maintainWorldPos
             */
            void setParent(SceneObject* parent, bool maintainWorldPos = true);

            /**
             * \return Parent of this SceneObject (NULL if no parent)
             */
            SceneObject* getParent() const;

            /**
             * Creates a new SceneObject with the specified name and adds it 
             * as a child of this SceneObject.
             *
             * Calling this method is functionally identical to:
             *
             *     OcularEngine.SceneManager()->createObject(name, thisObject);
             *
             * \param[in] name Name for the new object (does not have to be unique).
             * \return Pointer to the new object.
             */
            SceneObject* createChild(std::string const& name);

            /**
             * Adds the specified object as a child of this SceneObject and removes
             * it from it's previous parent.
             *
             * The object's parent will change, and it's state will be modified
             * to reflect it's new parent (active, forced visible, static/dynamic, etc.).
             *
             * May also specify whether or not to preserve the absolute world position.
             *
             * By default, the world position is maintained. This means that the local position
             * is modified such that, upon being added to the new parent, the world position 
             * remains the same. 
             *
             * Alternatively (by setting the maintainWorldPos parameter to FALSE), the local
             * position will remain the same but the absolute world position will be different.
             *
             * For example:
             *
             * We have a parentless SceneObject positioned at (0, 5, 0).
             * We assign it a parent at position (5, 0, 0).
             *
             * If maintainWorldPos is TRUE, then the SceneObject's absolute world position will
             * remain as (0, 5, 0), while its new local position to the parent is (-5, 5, 0)
             *
             * If maintainWorld is FALSE, then the SceneObject's local position to the parent 
             * will remain as (0, 5, 0), while it's new world position will be (5, 5, 0).
             *
             * \param[in] child
             * \param[in] maintainWorldPos
             */
            void addChild(SceneObject* child, bool maintainWorldPos = true);

            /**
             * \param[in] name Name to search for.
             * \return Pointer to first discovered child with specified name (NULL if not found).
             */
            SceneObject* findChild(std::string const& name);

            /**
             * \param[in] uuid UUID to search for.
             * \return Pointer to child with matching UUID (NULL if not found).
             */
            SceneObject* findChild(UUID const& uuid);

            /**
             * Removes the first discovered child with the specified name.
             *
             * This does not delete the child object, but simply removes it
             * from this SceneObject. The removed object will continue to exist,
             * along with all of it's routines, renderables, and children.
             *
             * A pointer to the removed object is returned. If you desired to
             * fully destroy the object, then you must call:
             *
             *     OcularEngine.SceneManager()->removeObject(object);
             *
             * Note that the removed object will retain the state of it's old
             * parent (active, forced visible, static/dynamic, etc.). It's transform
             * will also be unmodified, but may appear to move as it is no longer
             * being based on the parent but now on the world.
             *
             * \param[in] name
             * \return Pointer to the removed object (NULL if no matching child was found).
             */
            SceneObject* removeChild(std::string const& name);

            /**
             * Removes the first discovered child with the specified UUID.
             *
             * This does not delete the child object, but simply removes it
             * from this SceneObject. The removed object will continue to exist,
             * along with all of it's routines, renderables, and children.
             *
             * A pointer to the removed object is returned. If you desired to
             * fully destroy the object, then you must call:
             *
             *    OcularScene->removeObject(object);
             *
             * Note that the removed object will retain the state of it's old
             * parent (active, forced visible, static/dynamic, etc.). It's transform
             * will also be unmodified, but may appear to move as it is no longer
             * being based on the parent but now on the world.
             *
             * \param[in] uuid
             * \return Pointer to the removed object (NULL if no matching child was found).
             */
            SceneObject* removeChild(UUID const& uuid);

            /**
             * Removes the first discovered child with the specified name.
             *
             * This does not delete the child object, but simply removes it
             * from this SceneObject. The removed object will continue to exist,
             * along with all of it's routines, renderables, and children.
             *
             * A pointer to the removed object is returned. If you desired to
             * fully destroy the object, then you must call:
             *
             *     OcularScene->removeObject(object);
             *
             * Note that the removed object will retain the state of it's old
             * parent (active, forced visible, static/dynamic, etc.). It's transform
             * will also be unmodified, but may appear to move as it is no longer
             * being based on the parent but now on the world.
             *
             * \param[in] object
             * \return Pointer to the removed object (NULL if no matching child was found).
             */
            SceneObject* removeChild(SceneObject const* object);

            /**
             * \return The number of child objects owned by this SceneObject.
             */
            uint32_t getNumChildren() const;

            /**
             * \return A vector containing pointers to all children owned by this SceneObject.
             */
            std::vector<SceneObject*> const& getAllChildren() const;

            //------------------------------------------------------------
            // Routine Methods
            //------------------------------------------------------------

            /**
             * Adds a new instance of the specified routine to the SceneObject.
             *
             * \param[in] name Name of the ARoutine implementation to add.
             * \return TRUE if the ARoutine implementation was successfully added. If it fails,
             *         then no matching implementation with that name was discovered.
             */
            ARoutine* addRoutine(std::string const& name);

            /**
             *
             */
            void addRoutine(ARoutine* routine);

            /**
             * Adds a new instance of a routine to the SceneObject.
             *
             * The object takes full ownership of the routine and frees it when
             * no longer in use.
             */
            template<class T> T* addRoutine() 
            { 
                T* t = new T();
                ARoutine* routine = dynamic_cast<ARoutine*>(t);

                if(routine)
                {
                    routine->setParent(this);
                    m_Routines.push_back(routine);
                    routine->onCreation();
                }
                else
                {
                    // T is not a routine
                    delete t;
                    t = nullptr;
                }

                return t;
            }

            /**
             * Removes an instance of the specified routine implementation from the SceneObject.
             * Will only remove the first instance of the implementation that it finds.
             *
             * \param[in] name Name of the ARoutine implementation to remove.
             * \return TRUE if the routine implementation was successfully removed. If it fails,
             *         then no matching implementation instance with that name was discovered.
             */
            bool removeRoutine(std::string const& name);

            /**
             * Removes the specified ARoutine instance from the SceneObject.
             *
             * \param[in] routine Routine instance to remove from the SceneObject.
             * \param[in] transferring If true, then this Routine is being transferred to another
             *                         SceneObject and should not be deleted.
             *
             * \return TRUE if the routine instance was successfully discovered and removed.
             *         If it fails, then no matching instance was discovered.
             */
            bool removeRoutine(ARoutine* routine, bool transferring = false);

            /**
             * Removes all routine instances that belong to this SceneObject.
             */
            void removeAllRoutines();

            /**
             * Retrieves the first discovered instance of the specified ARoutine implementation.
             *
             * \param[in] name 
             * \return The ARoutine instance or NULL if no matching routines were discovered.
             */
            ARoutine* getRoutine(std::string const& name);

            /**
             * Returns a collection of all routines that are owned by this SceneObject.
             */
            std::vector<ARoutine*> const& getAllRoutines() const;

            /**
             *
             */
            uint32_t getNumRoutines() const;

            //------------------------------------------------------------
            // Renderable Methods
            //------------------------------------------------------------
            
            /**
             * Attempts to create a renderable with the specified type name and set it as the object's renderable.
             * The renderable, if successfully created, will be automatically initialized.
             *
             * \param[in] name Name of the renderable type. For example: "MeshRenderable". 
             *
             * \return The new renderable. Is NULL if creation failed (no renderable with matching type).
             */
            ARenderable* setRenderable(std::string const& name);

            /**
             * Sets the renderable for the object.
             *
             * It is assumed that the renderable has been already initialized or will be done so manually.
             *
             * \param[in] renderable Renderable for the object. May be NULL.
             */
            void setRenderable(ARenderable* renderable);

            /**
             * Sets the renderable of the specified type.
             * The renderable is automatically initialized.
             */ 
            template<class T>
            T* setRenderable()
            {
                T* t = new T();
                ARenderable* renderable = dynamic_cast<ARenderable*>(t);

                if(renderable)
                {
                    removeRenderable();
                    m_Renderable = renderable;
                    m_Renderable->m_Parent = this;
                    m_Renderable->initialize();
                }
                else
                {
                    delete t;
                    t = nullptr;
                }

                return t;
            }

            /**
             * Removes the renderable from the object.
             * The renderable will be destroyed if it is not being transferred.
             *
             * \param[in] transferring If TRUE the renderable is being transferred to another object and should not be destroyed.
             */
            void removeRenderable(bool transferring = false);

            /**
             * \return Renderable attached to this object. May be NULL.
             */
            ARenderable* getRenderable() const;

            //------------------------------------------------------------
            // Inherited Methods
            //------------------------------------------------------------

            virtual void onLoad(BuilderNode const* node) override;
            virtual void onSave(BuilderNode* node) const override;

            //------------------------------------------------------------
            // Bounds Related
            //------------------------------------------------------------

            /**
             * Forces the bounds of this SceneObject to be completely rebuilt and calculated.
             */
            void forceBoundsRebuild();

            /**
             * Updates the bounds due to the selected actions (translation, rotation, and/or scaling).
             * This is typically called automatically whenever needed.
             *
             * \param[in] dirtyFlags Actions that caused the bounds to be dirty and require a rebuild. See Math::Transform::DirtyFlags
             */
            virtual void updateBounds(uint32_t dirtyFlags);

            /**
             * Returns the bounding sphere for the object.
             *
             * \param[in] local If TRUE, returns the local untransformed bounds for the object (local space).
             *                  If FALSE, returns the bounds modified according to the transform (world space).
             */
            Math::BoundsSphere getBoundsSphere(bool local);

            /**
             * Returns the axis-aligned bounding box for the object.
             *
             * \param[in] local If TRUE, returns the local untransformed bounds for the object (local space).
             *                  If FALSE, returns the bounds modified according to the transform (world space).
             */
            Math::BoundsAABB getBoundsAABB(bool local);

            /**
             * Returns the orientated bounding box for the object.
             *
             * \param[in] local If TRUE, returns the local untransformed bounds for the object (local space).
             *                  If FALSE, returns the bounds modified according to the transform (world space).
             */
            Math::BoundsOBB getBoundsOBB(bool local);

        protected:

            void getModelMatrix(Math::Matrix4x4& matrix);
            void removeChild(std::vector<SceneObject*>::iterator& child);

            //------------------------------------------------------------

            SceneObject* m_Parent;
            uint32_t m_Layer;          ///< The render layer this object is part of.

            // Physical Characteristics

            Graphics::UniformPerObject m_UniformData;
            Math::Transform m_Transform;

			Math::BoundsSphere m_BoundsSphereLocal;
			Math::BoundsAABB   m_BoundsAABBLocal;
			Math::BoundsOBB    m_BoundsOBBLocal;
            Math::BoundsSphere m_BoundsSphereWorld;
            Math::BoundsAABB   m_BoundsAABBWorld;
            Math::BoundsOBB    m_BoundsOBBWorld;
            
            bool m_IsStatic;           ///< Boolean if this object is static. If static, no movement or rotation calls will have any affect. Determines which SceneTree it will reside in. Default: false.
            bool m_IsActive;           ///< If active, an object's Routines will be invoked. Default: true.
            bool m_IsVisible;          ///< If visible, an object's Renderables will be invoked. Default: false.
            bool m_ForcedVisible;      ///< If true, the object will be forced visible and the Renderable will always be invoked irregardless of any frustum, cull, etc. tests. Default: false.
            bool m_Persists;           ///< If true, this object (and children) will persist inbetween scenes. When a new scene is created, it will automatically be added to it.

            std::vector<ARoutine*> m_Routines;
            ARenderable* m_Renderable;

            std::vector<SceneObject*> m_Children;

            private:
        };
    }
    /**
     * @} End of Doxygen Groups
     */
}
/**
 * @} End of Doxygen Groups
 */

//------------------------------------------------------------------------------------------

#endif