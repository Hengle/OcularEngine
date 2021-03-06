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

#pragma once
#ifndef __H__OCULAR_EDITOR_PROPERTY_WIDGET__H__
#define __H__OCULAR_EDITOR_PROPERTY_WIDGET__H__

#include <cstdint>

#include <QtWidgets/qframe.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>

#include "ObjectIO/ExposedVariable.hpp"
#include "Utilities/VoidCast.hpp"

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
        /**
         * \class PropertyWidget
         * \brief Parent class for all general property widgets.
         *
         * Each instance of a property widget is broken into two parts: a name on 
         * the left side, and some form of data display on the right.
         */
        class PropertyWidget : public QFrame
        {
        public:

            PropertyWidget(std::string const& type, QWidget* parent = nullptr);
            ~PropertyWidget();

            virtual QSize sizeHint() const override;

            //------------------------------------------------------------

            /**
             * Update method that is called up to once a frame so that changes to the 
             * tracked variable may be checked for, and displayed.
             *
             * \return Should return TRUE if a property was modified by the user; otherwise return FALSE.
             */
            virtual bool updateProperties();

            /**
             * Sets the value of the variable and/or widget.
             */
            virtual void setValue(void* value, uint32_t size);

            /**
             * \return Value as a std::string
             */
            virtual std::string getValue() const;

            /**
             * \return Type of property as a string
             */
            std::string getType() const;

            /**
             * Sets the variable whose data should be displayed and made available to
             * modify by the implementation.
             *
             * \param[in] variable
             */
            void setVariable(Core::ExposedVariable& variable);

            /**
             * Sets the text of the label on the left-side of the widget.
             *
             * \param[in] name
             */
            void setDisplayName(std::string const& name);
            
            /**
             * Retrieves the text of the label on the left-side of the widget.
             */
            std::string const& getDisplayName() const;

            /**
             * Retrieves the name of the exposed variable.
             */
            std::string const& getVariableName() const;
            
            /**
             * Adds a new widget to the left-side layout.
             *
             * \param[in] widget
             */
            void addWidgetLeftSide(QWidget* widget);
            
            /**
             * Adds a new widget to the right-side layout.
             *
             * \param[in] widget
             */
            void addWidgetRightSide(QWidget* widget);

        protected:
            
            QHBoxLayout* m_LayoutRight;          ///< Layout for the right-side frame

            Core::ExposedVariable m_Variable;    ///< The variable this widget is responsible for displaying and modifying

        private:

            void createLeftSide();
            void createRightSide();
            void createLayout();

            //------------------------------------------------------------

            QFrame* m_FrameLeftSide;             ///< Frame for the name label. Fixed width of 75.
            QFrame* m_FrameRightSide;            ///< Frame for the implementation-specific widgets
            
            QHBoxLayout* m_Layout;               ///< Layout for the overall widget
            QHBoxLayout* m_LayoutLeft;           ///< Layout for the left-side frame

            QLabel* m_LabelName;

            std::string m_DisplayName;
            std::string m_Type;
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