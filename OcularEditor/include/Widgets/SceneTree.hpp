/**
 * Copyright 2014-2016 Steven T Sell (ssell@ocularinteractive.com)
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
#ifndef __H__OCULAR_EDITOR_SCENE_TREE__H__
#define __H__OCULAR_EDITOR_SCENE_TREE__H__

#include <QtWidgets/qtreewidget.h>
#include "Events/AEventListener.hpp"

//------------------------------------------------------------------------------------------

/**
 * \addtogroup Ocular
 * @{
 */
namespace Ocular
{
    /**
     * \addtogroup Editor
     * @{
     */
    namespace Editor
    {
        class SceneTreeItem;

        /**
         * \class SceneTree
         * \brief 
         */
        class SceneTree : public QTreeWidget, public Ocular::Core::AEventListener
        {
        public:

            SceneTree(QWidget* parent = nullptr);
            ~SceneTree();

            virtual QSize sizeHint() const override;

            /**
             * Retrieves the item representing the specified SceneObject.
             *
             * \param[in] object
             * \return NULL if no matching item was found
             */
            SceneTreeItem* getItem(Core::SceneObject* object);

            /**
             * Retrieves the item representing the SceneObject with the specified UUID.
             *
             * \param[in] uuid
             * \return NULL if no matching item was found
             */
            SceneTreeItem* getItem(Core::UUID const& uuid);

        protected:

            virtual bool onEvent(std::shared_ptr<Core::AEvent> event) override;

            void addObject(Core::SceneObject* object);
            bool isObjectTracked(Core::SceneObject* object);

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
