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
#include "Widgets/Standard/ComboBox.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Editor
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------
        
        ComboBox::ComboBox(QWidget* parent)
            : QComboBox(parent),
              m_WasEdited(false),
              m_ValidationIndex(-1)
        {
            connect(this, SIGNAL(activated(int)), this, SLOT(onIndexChanged(int)));
        }

        ComboBox::~ComboBox()
        {

        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        bool ComboBox::wasEdited(bool reset)
        {
            bool result = m_WasEdited;

            if(reset)
            {
                m_WasEdited = false;
            }

            return result;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------

        void ComboBox::onIndexChanged(int index)
        {
            if(index != m_ValidationIndex)
            {
                m_ValidationIndex = index;
                m_WasEdited = true;
            }
        }
    }
}