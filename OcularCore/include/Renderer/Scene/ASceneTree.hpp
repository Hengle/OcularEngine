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

#ifndef __H__OCULAR_CORE_ASCENE_TREE__H__
#define __H__OCULAR_CORE_ASCENE_TREE__H__

#include "SceneNode.hpp"

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
        /**
         * \class SceneTree
         */
        class ASceneTree
        {
        public:

            ASceneTree();
            ~ASceneTree();

            virtual void addNode(std::shared_ptr<SceneNode>& node) = 0;
            
            virtual std::shared_ptr<SceneNode> getNode(UUID const& uuid) const = 0;
            virtual std::shared_ptr<SceneNode> getNode(std::string const& name) const = 0;

            virtual void getAllNodes(std::list<std::shared_ptr<SceneNode>>& nodes) const = 0;
            virtual void getAllVisibleNodes(std::list<std::shared_ptr<SceneNode>>& nodes) const = 0;
            virtual void getAllActiveNodes(std::list<std::shared_ptr<SceneNode>>& nodes) const = 0;

        protected:

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