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
#ifndef __H__OCULAR_EDITOR_MAIN_WINDOW__H__
#define __H__OCULAR_EDITOR_MAIN_WINDOW__H__

#include <QtWidgets/QMainWindow>

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
        class MainMenuBar;
        class MainStatusBar;
        class MainFrame;
        class ContentTab;
        class ToolBarCommon;

        /**
         * \class MainWindow
         * \brief Main window for the Ocular Editor application
         */
        class MainWindow : public QMainWindow
        {
            Q_OBJECT

        public:

            MainWindow(QWidget* parent = nullptr);
            ~MainWindow();

            virtual QSize sizeHint() const override;

        protected:

        private:

            void setupLayouts();
            void setupMenus();
            void setupContentTab();

            //------------------------------------------------------------

            QVBoxLayout* m_LayoutMain;          ///< Layout for the entire window
            QVBoxLayout* m_LayoutToolBars;      ///< Layout for the tool bars
            QHBoxLayout* m_LayoutContent;       ///< Layout for the main content widgets (renderer, scene modifiers, etc.)

            MainMenuBar*   m_MenuBar;
            MainStatusBar* m_StatusBar;
            MainFrame*     m_MainFrame;

            ToolBarCommon* m_ToolBarCommon;

            ContentTab* m_ContentTab;
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
