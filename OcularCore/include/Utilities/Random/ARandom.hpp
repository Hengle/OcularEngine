/**
* Copyright 2014 Steven T Sell (ssell@ocularinteractive.com)
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
#ifndef __H__OCULAR_UTILS_ARANDOM__H__
#define __H__OCULAR_UTILS_ARANDOM__H__

//------------------------------------------------------------------------------------------

/**
* \addtogroup Ocular
* @{
*/
namespace Ocular
{
    /**
    * \addtogroup Utils
    * @{
    */
    namespace Utils
    {
        /**
        * \addtogroup Random
        * @{
        */
        namespace Random
        {
            /**
             * \class ARandom
             */
            class ARandom
            {
            public:

                ARandom();
                ~ARandom();

                /**
                 * Seeds the PRNG with the current epoch time (NS)
                 */
                void seed();

                /**
                 * Seeds the PRNG with the specified seed value.
                 */
                virtual void seed(long long seed);

                /**
                 * Retrieves the next pseudo-random number (unbounded).
                 */
                virtual unsigned next() = 0;

                /**
                 * Retrieves the next pseudo-random number and fits it inside of the specified bounds (this is not a clamp)
                 */
                virtual unsigned next(unsigned min, unsigned max);

            protected:

                long long m_Seed;

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
}
/**
* @} End of Doxygen Groups
*/

//------------------------------------------------------------------------------------------

#endif