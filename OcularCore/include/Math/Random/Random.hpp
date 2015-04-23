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
#ifndef __H__OCULAR_MATH_RANDOM__H__
#define __H__OCULAR_MATH_RANDOM__H__

#include "ARandom.hpp"
#include <memory>

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
        /**
         * \addtogroup Random
         * @{
         */
        namespace Random
        {
            enum PRNGImplementation
            {
                MersenneTwister = 0,
                TinyMersenneTwister,
                CMWC,
                WELL,
                XorShift
            };
            
            /**
             * Creates an instance of the specified PRNG.
             * The PRNG will be seeded with the current time.
             *
             * \return A shared smart pointer of the PRNG instance.
             */
            std::shared_ptr<ARandom> CreatePRNG(PRNGImplementation prng);
            
            /**
             * Creates an instance of the specified PRNG.
             *
             * \param[in] seed Seed value to initialize the PRNG with.
             * \return A shared smart pointer of the PRNG instance.
             */
            std::shared_ptr<ARandom> CreatePRNG(PRNGImplementation prng, int64_t seed);
        }
        /**
         * @} End of Doxygen Groups
         */
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