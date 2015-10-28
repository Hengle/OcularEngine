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

#pragma once
#ifndef __H__OCULAR_RENDERER_WINDOW__H__
#define __H__OCULAR_RENDERER_WINDOW__H__

#include "Object.hpp"
#include "WindowManager.hpp"
#include "WindowDisplay.hpp"
#include "WindowDescriptor.hpp"

#include "Graphics/Texture/RenderTexture.hpp"

#include <string>

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
        class RenderContext;

        /**
         * \class Window
         * 
         * Abstract parent class of all Window implementations.<br/><br/>
         *
         * See ::WindowWin32 <br/>
         * See ::WindowLinux <br/>
         * See ::WindowOSX
         */
        class AWindow : public Object
        {
            friend class WindowManager;
        public:

            /**
             * \param name        Display name of the window
             * \param width       Width of the window 
             * \param height      Height of the window
             * \param colorBits   Number of color bits
             * \param depthBits   Number of depth bits
             * \param stencilBits Number of stencil bits
             * \param display     Display mode
             * \param alwaysOnTop Should this Window be rendered on top of other Windows? (Exclusive mode rendering)
             */
            AWindow(WindowDescriptor descriptor);

            virtual ~AWindow();

            //--------------------------------------------

            /**
             * \return The WindowDescriptor for this window instance.
             */
            WindowDescriptor getDescriptor() const;

            /**
             * Sets the WindowDescriptor for this window instance.
             * \param[in] descriptor
             */
            void setDescriptor(WindowDescriptor const& descriptor);

            /**
             * \return TRUE if this window currently has input focus.
             */
            bool hasFocus() const;

            /**
             * \return The current backbuffer RenderTexture.
             */
            Graphics::RenderTexture const* getRenderTexture() const;

            /**
             * Sets the RenderTexture to use as the backbuffer.
             * 
             * This method is typically called automatically during window creation
             * and/or GraphicsDriver initialization. Except for special circumstances
             * it not be called externally.
             *
             * \note When set, the window assumes ownership of the texture and will 
             *       destroy it when it is no longer in use.
             *
             * \param[in] renderTexture
             */
            void setRenderTexture(Graphics::RenderTexture* renderTexture);

        protected:

            /**
             * Opens and initializes the Window.
             */
            virtual void open() = 0;

            /**
             * Updates and recreates the Window.
             */
            virtual void update(uint64_t time) = 0;

            /**
             * Closes the Window.
             */
            virtual void close() = 0;

            //----------------------------------------

            WindowDescriptor m_Descriptor;
            Graphics::RenderTexture* m_RenderTexture;

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