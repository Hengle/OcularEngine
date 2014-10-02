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
#ifndef __H__OCULAR_UTILS_RANDOM_TINY_MERSENNE_TWISTER__H__
#define __H__OCULAR_UTILS_RANDOM_TINY_MERSENNE_TWISTER__H__

#include "ARandom.hpp"

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
            * \class TinyMersenneTwister
            *
            * Implementation of the 127 periodicity variation of the Tiny Mersenne Twister PRNG using the IRandom interface.<br/>
            * The Tiny implementation is faster and smaller in state than the traditional Mersenne Twister (see MersenneTwister19937).
            *
            * Original implementation may be found at: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/TINYMT/index.html
            * Copyright Mutsuo Saito and Makoto Matsumoto
            */
            class TinyMersenneTwister : public ARandom
            {
            public:

                TinyMersenneTwister();
                ~TinyMersenneTwister();

                virtual unsigned next();

            protected:

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