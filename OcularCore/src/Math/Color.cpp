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

#include "Math/Color.hpp"
#include "Math/MathCommon.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Core
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        Color::Color(Math::Vector4f const& color)
        {
            r = Math::Clamp<float>(color.r);
            g = Math::Clamp<float>(color.g);
            b = Math::Clamp<float>(color.b);
            a = Math::Clamp<float>(color.a);
        }

        Color::Color(float r, float g, float b, float a)
        {
            this->r = Math::Clamp<float>(r);
            this->g = Math::Clamp<float>(g);
            this->b = Math::Clamp<float>(b);
            this->a = Math::Clamp<float>(a);
        }

        Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            this->r = static_cast<float>(r) / 255.0f;
            this->g = static_cast<float>(g) / 255.0f;
            this->b = static_cast<float>(b) / 255.0f;
            this->a = static_cast<float>(a) / 255.0f;
        }

        Color::Color()
        {
            // Default set to the ErrorPink color
            r = 1.0f;
            g = 0.078f; 
            b = 0.576f;
            a = 1.0f;
        }

        Color::~Color()
        {
        
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        Color const& Color::Brown()
        {
            static const Color result(0.545f, 0.27f, 0.075f); 
            return result;
        }

        Color const& Color::Black()
        {
            static const Color result(0.0f, 0.0f, 0.0f); 
            return result;
        }

        Color const& Color::Blue()
        {
            static const Color result(0.0f, 0.0f, 1.0f); 
            return result;
        }

        Color const& Color::Clear()
        {
            static const Color result(0.0f, 0.0f, 0.0f, 0.0f); 
            return result;
        }

        Color const& Color::Cyan()
        {
            static const Color result(0.0f, 1.0f, 1.0f); 
            return result;
        }

        Color const& Color::DarkGray()
        {
            static const Color result(0.2f, 0.2f, 0.2f); 
            return result;
        }

        Color const& Color::ErrorPink()
        {
            static const Color result(1.0f, 0.078f, 0.576f); 
            return result;
        }

        Color const& Color::FrigidBlue()
        {
            static const Color result(0.153f, 0.863f, 0.973f);
            return result;
        }

        Color const& Color::Gold()
        {
            static const Color result(1.0f, 0.843f, 0.0f); 
            return result;
        }

        Color const& Color::Gray()
        {
            static const Color result(0.5f, 0.5f, 0.5f); 
            return result;
        }

        Color const& Color::Green()
        {
            static const Color result(0.0f, 1.0f, 0.0f); 
            return result;
        }

        Color const& Color::Magenta()
        {
            static const Color result(1.0f, 0.0f, 1.0f); 
            return result;
        }

        Color const& Color::Maroon()
        {
            static const Color result(0.502f, 0.0f, 0.0f); 
            return result;
        }

        Color const& Color::Orange()
        {
            static const Color result(1.0f, 0.647f, 0.0f); 
            return result;
        }

        Color const& Color::Pink()
        {
            static const Color result(1.0f, 0.753f, 0.796f); 
            return result;
        }

        Color const& Color::Purple()
        {
            static const Color result(0.294f, 0.0f, 0.51f); 
            return result;
        }

        Color const& Color::Red()
        {
            static const Color result(1.0f, 0.0f, 0.0f); 
            return result;
        }

        Color const& Color::Silver()
        {
            static const Color result(0.753f, 0.753f, 0.753f); 
            return result;
        }

        Color const& Color::White()
        {
            static const Color result(1.0f, 1.0f, 1.0f); 
            return result;
        }

        Color const& Color::Yellow()
        {
            static const Color result(1.0f, 1.0f, 0.0f); 
            return result;
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}