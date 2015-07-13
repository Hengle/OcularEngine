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

#pragma once
#ifndef __H__OCULAR_CORE_SCENE__H__
#define __H__OCULAR_CORE_SCENE__H__

//------------------------------------------------------------------------------------------

/**
 * \addtogroup Ocular
 * @{
 */
namespace Ocular
{
    /**
     * \addtogroup Core
     * @{
     */
    namespace Core
    {
        class SceneManager;
        class ISceneTree;

        /**
         * \class Scene
         *
         * A Scene something something
         */
        class Scene
        {
            friend class SceneManager;

        public:

            ~Scene();

            void addObject(SceneObject* object);
            void removeObject(SceneObject* object);

            /**
             * Envokes the various update methods for all SceneObjects in the current SceneTree.
             */
            void update();

            /**
             * Envokes the various render methods for all SceneObjects in the current SceneTree.
             */
            void render();

        protected:

            Scene();

        private:

            ISceneTree* m_SceneTree;
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