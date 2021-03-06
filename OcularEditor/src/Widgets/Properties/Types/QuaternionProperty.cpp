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

#include "stdafx.h"
#include "Widgets/Properties/Types/QuaternionProperty.hpp"
#include "Widgets/Properties/PropertyWidgetRegistrar.hpp"
#include "Math/Quaternion.hpp"

OCULAR_REGISTER_PROPERTY_WIDGET(Ocular::Editor::QuaternionProperty, OCULAR_TYPE_NAME(Ocular::Math::Quaternion));

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Editor
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------
        
        QuaternionProperty::QuaternionProperty(QWidget* parent)
            : PropertyWidget(OCULAR_TYPE_NAME(Ocular::Math::Quaternion), parent)
        {
            m_LabelW = new QLabel("W");
            m_LabelX = new QLabel("X");
            m_LabelY = new QLabel("Y");
            m_LabelZ = new QLabel("Z");
            
            m_EditW = new LineEdit(LineType::Float);
            m_EditX = new LineEdit(LineType::Float);
            m_EditY = new LineEdit(LineType::Float);
            m_EditZ = new LineEdit(LineType::Float);
            
            m_LayoutRight->addWidget(m_LabelW);
            m_LayoutRight->addWidget(m_EditW);
            m_LayoutRight->addWidget(m_LabelX);
            m_LayoutRight->addWidget(m_EditX);
            m_LayoutRight->addWidget(m_LabelY);
            m_LayoutRight->addWidget(m_EditY);
            m_LayoutRight->addWidget(m_LabelZ);
            m_LayoutRight->addWidget(m_EditZ);
        }

        QuaternionProperty::~QuaternionProperty()
        {

        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------
        
        bool QuaternionProperty::updateProperties()
        {
            bool result = false;

            if(m_Variable.data)
            {
                Math::Quaternion* quaternion = void_cast<Math::Quaternion*>(m_Variable.data);

                if(m_EditW->wasEdited())
                {
                    (*quaternion).w() = m_EditW->asFloat();
                    result = true;
                }
                else if(!m_EditW->hasFocus())
                {
                    m_EditW->setText(OcularString->toString<float>(quaternion->w()).c_str());
                }

                if(m_EditX->wasEdited())
                {
                    (*quaternion).x() = m_EditX->asFloat();
                    result = true;
                }
                else if(!m_EditX->hasFocus())
                {
                    m_EditX->setText(OcularString->toString<float>(quaternion->x()).c_str());
                }

                if(m_EditY->wasEdited())
                {
                    (*quaternion).y() = m_EditY->asFloat();
                    result = true;
                }
                else if(!m_EditY->hasFocus())
                {
                    m_EditY->setText(OcularString->toString<float>(quaternion->y()).c_str());
                }

                if(m_EditZ->wasEdited())
                {
                    (*quaternion).z() = m_EditZ->asFloat();
                    result = true;
                }
                else if(!m_EditZ->hasFocus())
                {
                    m_EditZ->setText(OcularString->toString<float>(quaternion->z()).c_str());
                }
            }

            return result;
        }

        void QuaternionProperty::setValue(void* value, uint32_t const size)
        {
            if(value && (size == sizeof(Math::Euler)))
            {
                Math::Quaternion valueCast = void_cast<Math::Quaternion>(value);

                if(m_Variable.data)
                {
                    Math::Quaternion* valuePtr = void_cast<Math::Quaternion*>(m_Variable.data);

                    if(valuePtr)
                    {
                        (*valuePtr) = valueCast;
                    }
                }

                m_EditW->setText(OcularString->toString<float>(valueCast.w()).c_str());
                m_EditX->setText(OcularString->toString<float>(valueCast.x()).c_str());
                m_EditY->setText(OcularString->toString<float>(valueCast.y()).c_str());
                m_EditZ->setText(OcularString->toString<float>(valueCast.z()).c_str());
            }
        }

        std::string QuaternionProperty::getValue() const
        {
            auto qstr = m_EditW->text() + " " + m_EditX->text() + " " + m_EditY->text() + " " + m_EditZ->text();
            return qstr.toStdString();
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------
        
        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}