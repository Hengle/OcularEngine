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
#include "Widgets/Properties/Vector4Property.hpp"
#include "Math/Vector4.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Editor
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------
        
        Vector4Property::Vector4Property(QString const& displayName, QWidget* parent)
            : PropertyWidget(displayName, parent)
        {
            m_LabelX = new QLabel("X");
            m_LabelY = new QLabel("Y");
            m_LabelZ = new QLabel("Z");
            m_LabelW = new QLabel("W");
            
            m_EditX = new QLineEdit();
            m_EditY = new QLineEdit();
            m_EditZ = new QLineEdit();
            m_EditW = new QLineEdit();
            
            m_LayoutRight->addWidget(m_LabelX);
            m_LayoutRight->addWidget(m_EditX);
            m_LayoutRight->addWidget(m_LabelY);
            m_LayoutRight->addWidget(m_EditY);
            m_LayoutRight->addWidget(m_LabelZ);
            m_LayoutRight->addWidget(m_EditZ);
            m_LayoutRight->addWidget(m_LabelW);
            m_LayoutRight->addWidget(m_EditW);
        }

        Vector4Property::~Vector4Property()
        {

        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------
        
        void Vector4Property::updateProperties()
        {
            if(m_Variable.data)
            {
                Math::Vector4f vector = void_cast<Math::Vector4f>(m_Variable.data);

                if(!m_EditX->hasFocus())
                {
                    m_EditX->setText(OcularString->toString<float>(vector.x).c_str());
                }

                if(!m_EditY->hasFocus())
                {
                    m_EditY->setText(OcularString->toString<float>(vector.y).c_str());
                }

                if(!m_EditZ->hasFocus())
                {
                    m_EditZ->setText(OcularString->toString<float>(vector.z).c_str());
                }

                if(!m_EditW->hasFocus())
                {
                    m_EditW->setText(OcularString->toString<float>(vector.w).c_str());
                }
            }
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------
        
        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}