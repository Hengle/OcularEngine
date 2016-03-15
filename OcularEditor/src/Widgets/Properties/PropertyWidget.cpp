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
#include "Widgets/Properties/PropertyWidget.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Editor
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------
        
        PropertyWidget::PropertyWidget(QString const& displayName, QWidget* parent)
            : QFrame(parent)
        {
            setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

            //------------------------------------------------------------
            // Left Side
            //------------------------------------------------------------

            m_LabelName = new QLabel(displayName);
            
            m_LayoutLeft = new QHBoxLayout();
            m_LayoutLeft->addWidget(m_LabelName);
            
            m_FrameLeftSide = new QFrame();
            m_FrameLeftSide->setLayout(m_LayoutLeft);
            m_FrameLeftSide->setFixedWidth(75);

            //------------------------------------------------------------
            // Right Side
            //------------------------------------------------------------

            m_LayoutRight = new QHBoxLayout();
            m_FrameRightSide = new QFrame();
            m_FrameRightSide->setLayout(m_LayoutRight);

            //------------------------------------------------------------
            // Main Layout
            //------------------------------------------------------------

            m_Layout = new QHBoxLayout();
            m_Layout->setContentsMargins(5, 0, 5, 0);
            m_Layout->addWidget(m_FrameLeftSide);
            m_Layout->addWidget(m_FrameRightSide);

            setLayout(m_Layout);
        }

        PropertyWidget::~PropertyWidget()
        {

        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        QSize PropertyWidget::sizeHint() const
        {
            return QSize(275, 30);
        }

        void PropertyWidget::setVariable(Core::ExposedVariable& variable)
        {
            m_Variable = variable;
        }
        
        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------
        
        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}