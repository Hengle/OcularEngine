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
#include "Widgets/Properties/Types/ResourceProperty.hpp"
#include "Widgets/Properties/PropertyWidgetRegistrar.hpp"

OCULAR_REGISTER_PROPERTY_WIDGET(Ocular::Editor::ResourceProperty, Ocular::Utils::TypeName<Ocular::Core::Resource>::name);

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Editor
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------
        
        ResourceProperty::ResourceProperty(QWidget* parent)
            : PropertyWidget(parent)
        {
            m_LineValue = new LineEdit(LineType::String);
            m_ButtonBrowse = new ButtonResourceBrowse();

            m_LayoutRight->addWidget(m_LineValue);
            m_LayoutRight->addWidget(m_ButtonBrowse);
        }

        ResourceProperty::~ResourceProperty()
        {

        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------
        
        bool ResourceProperty::updateProperties()
        {
            bool result = false;

            if(m_Variable.data)
            {
                Core::Resource** value = (Core::Resource**)(m_Variable.data);
                
                if(m_ButtonBrowse->wasEdited())
                {
                    *value = OcularResources->getResource(m_ButtonBrowse->getSelectedResource());
                    m_LineValue->setText(m_ButtonBrowse->getSelectedResource().c_str());
                    
                    m_LineValue->setInvalid(false);
                    result = true;
                }
                else
                {
                    if(m_LineValue->wasEdited(false) && !m_LineValue->hasFocus())
                    {
                        m_LineValue->wasEdited();    // Ugly, reset the edited flag
                        const std::string resourceName = m_LineValue->text().toStdString();

                        if(OcularResources->doesExist(resourceName))
                        {
                            *value = OcularResources->getResource(m_LineValue->text().toStdString());

                            m_LineValue->setInvalid(false);
                            result = true;
                        }
                        else
                        {
                            m_LineValue->setInvalid(true);
                        }
                    }
                    else if(!m_LineValue->hasFocus())
                    {
                        Core::Resource* resource = *value;

                        if(resource)
                        {
                            m_LineValue->setText(resource->getMappingName().c_str());
                        }
                    }
                }
            }

            return result;
        }

        void ResourceProperty::setResourceType(Core::ResourceType const type)
        {
            m_ButtonBrowse->setResourceType(type);
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}