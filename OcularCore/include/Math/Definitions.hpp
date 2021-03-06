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
#ifndef __H__OCULAR_ENGINE_MATH_DEFINITIONS__H__
#define __H__OCULAR_ENGINE_MATH_DEFINITIONS__H__

//------------------------------------------------------------------------------------------

/**
 * \addtogroup Ocular
 * @{
 */
namespace Ocular
{
    /**
     * \addtogroup Math
     * @{
     */
    namespace Math
    {
        static const double ONE_OVER_TWO = 0.5;

        static const double PI           = 3.14159265;
        static const double PI_TWO       = 6.28318531;
        static const double PI_OVER_TWO  = 1.57079632;
        static const double PI_OVER_180  = 0.01745329;
        static const double PI_UNDER_180 = 57.2957796;
        static const double PHI          = 1.61803399;

        static const double EPSILON_DOUBLE = 0.0001;
        static const float  EPSILON_FLOAT  = 0.0001f;

        static const float RAY_LINE_LENGTH = 1000000000000.0f;    ///< When operations require a ray to be converted to a line, this is the length of the line used.

        static const float NORMAL_RANGE      = 1000000000000.0f;  ///< Assumed maximum range in a scene. Used for value normalization when calculation morton codes.
        static const float HALF_NORMAL_RANGE =  500000000000.0f;
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