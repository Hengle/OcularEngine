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
#ifndef __H__OCULAR_MATH_ARANDOM__H__
#define __H__OCULAR_MATH_ARANDOM__H__

#include <cstdint>

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
            /**
             * \class ARandom
             * 
             * Default interface for Ocular pseudo-random number generator implementations.
             * These generators support both integer and floating point output and can automatically
             * constrain them to a specified range.
             *
             * All implementations provide uniform output distribution. If a specific distribution
             * is required, such as gaussian, poisson, exponential, etc. then these generators may
             * be used with the STL distribution templates. Example:
             *
             *     #include <random>
             *
             *     using namespace Ocular::Math::Random;
             *     using namespace std;
             *
             *     void foo()
             *     {
             *         MersenneTwister19937 generator;
             *         poisson_distribution<uint32_t> distribution(10);
             *
             *         uint32_t value = distribution(generator);
             *     }
             */
            class ARandom
            {
            public:

                ARandom();
                ~ARandom();

                /**
                 * Identical to ARandom::next
                 */
                uint32_t operator()();

                /**
                 * Seeds the PRNG with the current epoch time (NS)
                 */
                void seed();

                /**
                 * Seeds the PRNG with the specified seed value.
                 * \param[in] seed
                 */
                virtual void seed(int64_t seed);

                /**
                 * Retrieves the next pseudo-random number (unbounded).
                 */
                virtual uint32_t next() = 0;

                /**
                 * Retrieves the next pseudo-random number and fits it inside of the specified bounds (this is not a clamp)
                 * 
                 * \param[in] min Minimum value for the random number
                 * \param[in] max Maximum value for the random number
                 */
                virtual uint32_t next(uint32_t min, uint32_t max);

                /**
                 * Retrieves the next pseudo-random number as a float in the
                 * the range of [0.0, 1.0]. The precision dictates the, well uh,
                 * precision of the returned value. 
                 *
                 * \param[in] precision
                 */
                virtual float nextf(float precision = 0.0001f);

                /**
                 * Retrieves the next pseudo-random number as a float in the
                 * specified range of [min, max]. 
                 *
                 * \param[in] min Minimum value to be returned.
                 * \param[in] max Maximum value to be returned.
                 * \param[in] precision
                 */
                virtual float nextf(float min, float max, float precision = 0.0001f);

                virtual int32_t nextSigned();

                virtual int32_t nextSigned(int32_t min, int32_t max);

                /**
                 * Retrieves the minimum value generated by this PRNG. Default of 0.
                 */
                virtual uint32_t min();

                /**
                 * Retrieves the minimum value generated by this PRNG. Default of UINT32_MAX.
                 */
                virtual uint32_t max();

            protected:

                int64_t m_Seed;

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