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

#include "stdafx.h"
#include "Widgets/SceneTreeItem.hpp"
#include "Widgets/SceneTree.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Editor
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        SceneTreeItem::SceneTreeItem(SceneTree* parent, Core::SceneObject* object)
            : QTreeWidgetItem(parent),
              m_Object(object)
        {
            buildItem();
        }

        SceneTreeItem::SceneTreeItem(SceneTreeItem* parent, Core::SceneObject* object)
            : QTreeWidgetItem(parent),
              m_Object(object)
        {
            buildItem();
        }

        SceneTreeItem::~SceneTreeItem()
        {
            m_Object = nullptr;
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        Core::SceneObject* SceneTreeItem::getObject() const
        {
            return m_Object;
        }

        Core::UUID const& SceneTreeItem::getUUID() const
        {
            return m_ObjectUUID;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        void SceneTreeItem::buildItem()
        { 
            if(m_Object)
            {
                m_ObjectUUID = m_Object->getUUID();

                setText(0, QString(m_Object->getName().c_str()));
                setText(1, QString(m_ObjectUUID.toString().c_str()));
            }
            else
            {
                setText(0, QString("NULL"));
                setText(1, QString("0"));
            }
        }

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}