/**
 * Copyright 2014-2015 Steven T Sell (ssell@ocularinteractive.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Scene/BVHSceneTree.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Core
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        BVHSceneTree::BVHSceneTree()
        {
        
        }

        BVHSceneTree::~BVHSceneTree()
        {
        
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        void BVHSceneTree::restructure()
        {
            
        }
        
        void BVHSceneTree::destroy()
        {
            
        }
        
        void BVHSceneTree::addObject(SceneObject* object)
        {
            if(object)
            {
                m_NewObjects.emplace_back(object);
            }
        }

        void BVHSceneTree::addObjects(std::vector<SceneObject*> const& objects)
        {
            for(auto object : objects)
            {
                if(object)
                {
                    m_NewObjects.emplace_back(object);
                }
            }
        }
        
        void BVHSceneTree::removeObject(SceneObject* object)
        {
        
        }
        
        void BVHSceneTree::getAllObjects(std::vector<SceneObject*>& objects) const
        {
        
        }
        
        void BVHSceneTree::getAllVisibleObjects(std::vector<SceneObject*>& objects) const
        {
        
        }

        void BVHSceneTree::getAllActiveObjects(std::vector<SceneObject*>& objects) const
        {
        
        }

        void BVHSceneTree::getIntersections(Math::Ray const& ray, std::vector<SceneObject*>& objects) const
        {
        
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        bool BVHSceneTree::rebuildNeeded() const
        {
            bool result = false;

            if(m_NewObjects.size() > 0)
            {
                result = true;
            }
            else
            {
                
            }

            return result;
        }

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------

    }
}