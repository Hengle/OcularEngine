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
#ifndef __H__OCULAR_UTILITIES_TYPES__H__
#define __H__OCULAR_UTILITIES_TYPES__H__

#include <cstdint>
#include <string>

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
         * Generates a unique type id value.
         */
        extern uint32_t GenerateTypeID();

        /**
         * \struct TypeInfo
         * \brief Utility structure to convert a type T to a string representation.
         *
         * In order to perform the conversion, an appropriate string name must have 
         * been provided via the OCULAR_REGISTER_TYPE or OCULAR_REGISTER_TYPE_CUSTOM
         * macros. If the type has not been properly registered, then a compile-time
         * error will be generated.
         *
         * Example of use:
         *
         *     std::cout << TypeInfo<float>::name << std::endl;
         *     std::cout << TypeInfo<int32_t>::name << std::endl;
         *     std::cout << TypeInfo<Ocular::Math::Matrix4x4>::name << std::endl;
         *
         *     // float
         *     // int
         *     // Matrix4x4
         *
         * This structure, and the associated macros, are used instead of built-in
         * approaches (such as typeid) as those other approaches will not give 
         * uniform results across all platforms.
         *
         * For example, using typeid on MSVC will return a human-readable string
         * but it will also include 'class ' or 'struct ', etc. While using it on
         * GCC will give a distinctly non-readable string output.
         *
         * A unique integer id is also generated for each type to allow for easier
         * comparisons of types. Example:
         *
         *     TypeInfo<int32_t>::id
         *
         * See also the helper macros:
         *
         *     OCULAR_TYPE_NAME(int32_t)
         *     OCULAR_TYPE_ID(int32_t)
         */
        template<typename T>
        struct TypeInfo 
        { 
            static std::string const& GetName();
            static uint32_t const& GetID();
        };
    }
    /**
     * @} End of Doxygen Groups
     */
}
/**
 * @} End of Doxygen Groups
 */


/**
 * Registers a type with a string name representation.
 *
 * This variant uses the explicit type name as the string name, 
 * and is preferred when registering primitives.
 *
 * See the other macro variant for custom naming.
 * 
 * When querying for a type name, use the OCULAR_TYPE_NAME macro.
 *
 * \code
 *     OCULAR_REGISTER_TYPE(int);
 *
 *     std::cout << "Type: " << OCULAR_TYPE_NAME(int) << std::endl;
 *     // "Type: int"
 * \endcode
 */
#define OCULAR_REGISTER_TYPE(X) template<> struct Ocular::Utils::TypeInfo<X> {                       \
                                    static std::string const& GetName() {                            \
                                        static const std::string name{ #X };                         \
                                        return name;                                                 \
                                    }                                                                \
                                    static uint32_t const& GetID() {                                 \
                                        static const uint32_t id{ Ocular::Utils::GenerateTypeID() }; \
                                        return id;                                                   \
                                    } }

/**
 * Registers a type with a custom string name represenation.
 *
 * This variant allows for the string name to be specified in
 * order to avoid lengthy namespace chains, etc. To register
 * the explicit type name, see OCULAR_REGISTER_TYPE.
 *
 * When querying for a type name, use the OCULAR_TYPE_NAME macro.
 * 
 * \code
 *     OCULAR_REGISTER_TYPE_CUSTOM(Ocular::Math::Matrix3x3, "Matrix3x3");
 *
 *     std::cout << "Type: " << OCULAR_TYPE_NAME(Ocular::Math::Matrix3x3) << std::endl;
 *     // "Type: Matrix3x3"
 * \endcode
 */
#define OCULAR_REGISTER_TYPE_CUSTOM(X,Y) template<> struct Ocular::Utils::TypeInfo<X> {                      \
                                            static std::string const& GetName() {                            \
                                                static const std::string name{ Y };                          \
                                                return name;                                                 \
                                            }                                                                \
                                            static uint32_t const& GetID() {                                 \
                                                static const uint32_t id{ Ocular::Utils::GenerateTypeID() }; \
                                                return id;                                                   \
                                            } }

/**
* Given a type, it will give back a string representation of that type.
*
* \code
*     std::cout << "Type: " << OCULAR_TYPE_NAME(float) << std::endl;
*     // "Type: float"
* \endcode
*
* \note If there is no registered name for the specified type, there
*       will be a compilation error. See the OCULAR_REGISTER_TYPE macros
*       for custom type registering.
*/
#define OCULAR_TYPE_NAME(X) Ocular::Utils::TypeInfo<X>::GetName()

/**
* Given a type or variable, it will give back a string representation of that type.
*
* \code
*     const int a = 0;
*     const Matrix3x3 matrix;
*
*     std::cout << "Type: " << OCULAR_TYPE(a) << std::endl;
*     std::cout << "Type: " << OCULAR_TYPE(matrix) << std::endl;
*
*     // "Type: int"
*     // "Type: Matrix3x3"
* \endcode
*
* \note If there is no registered name for the specified type, there
*       will be a compilation error. See the OCULAR_REGISTER_TYPE macros
*       for custom type registering.
*/
#define OCULAR_TYPE(X) OCULAR_TYPE_NAME(decltype(X))

/**
* Given a type, it will give back an integer id associated with that type.
*
* \code
*     std::cout << "ID: " << OCULAR_TYPE_ID(float) << std::endl;
*     // "ID: 6"
* \endcode
*
* \note The integer ids are only guaranteed to be unique and consistent for a single run.
*       Each run of the application may assign different id values to the types (though
*       the ids will be consistent for the entirety of the that run).
*
* \note If there is no registered name for the specified type, there
*       will be a compilation error. See the OCULAR_REGISTER_TYPE macros
*       for custom type registering.
*/
#define OCULAR_TYPE_ID(X) Ocular::Utils::TypeInfo<X>::GetID()

//------------------------------------------------------------------------------------------
// Register Common Types
//------------------------------------------------------------------------------------------

OCULAR_REGISTER_TYPE(int8_t);
OCULAR_REGISTER_TYPE(uint8_t);
OCULAR_REGISTER_TYPE(int16_t);
OCULAR_REGISTER_TYPE(uint16_t);
OCULAR_REGISTER_TYPE(int32_t);
OCULAR_REGISTER_TYPE(uint32_t);
OCULAR_REGISTER_TYPE(int64_t);
OCULAR_REGISTER_TYPE(uint64_t);
OCULAR_REGISTER_TYPE(bool);
OCULAR_REGISTER_TYPE(float);
OCULAR_REGISTER_TYPE(double);
OCULAR_REGISTER_TYPE(long double);

OCULAR_REGISTER_TYPE_CUSTOM(std::string, "String");

//------------------------------------------------------------------------------------------

#endif