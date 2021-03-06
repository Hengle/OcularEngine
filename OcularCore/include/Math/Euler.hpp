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

#ifndef __H__OCULAR_MATH_EULER_H__
#define __H__OCULAR_MATH_EULER_H__

#include "Utilities/TypeInfo.hpp"

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
        //----------------------------------------------------------------
        // Forward Declarations

        template<typename T> class Vector3;
        class Matrix3x3;
        class Quaternion;

        //----------------------------------------------------------------

        /**
         * \class Euler
         * 
         * Implementation of Euler (Tait-Bryan) angles - yaw, pitch, roll.
         * In NASA Standard Aeroplace notation, these correspond to heading, altitude, and bearing respectively.
         *
         * All input and output from the Euler class is in degrees, but internally
         * the components are stored as radians. 
         */
        class Euler
        {
            friend class Matrix3x3;
            friend class Quaternion;

        public:

            /**
             *
             */
            Euler();

            /**
             * \param[in] pPitch Rotation (in degrees) around the local x-axis
             * \param[in] pYaw   Rotation (in degrees) around the local y-axis
             * \param[in] pRoll  Rotation (in degrees) around the local z-axis
             */
            Euler(float const pPitch, float const pYaw, float const pRoll);

            /**
             * \param[in] vector Vector expected in form of (Pitch, Yaw, Roll) or (X-Rotation, Y-Rotation, Z-Rotation)
             */
            Euler(Vector3<float> const& vector);

            /**
             * Initializes the Euler angles from a rotation matrix.
             *
             * \param[in] rotationMatrix
             */
            Euler(Matrix3x3 const& rotationMatrix);

            /**
             * Initializes the Euler angles from a quaternion.
             * \param[in] quaternion
             */
            Euler(Quaternion const& quaternion);

            ~Euler();

            //------------------------------------------------------------
            // GETTERS / SETTERS
            //------------------------------------------------------------
            
            /**
             * \return The Yaw in degrees.
             */
            float getYaw() const;

            /**
             * \return The Yaw in radians.
             */
            float getYawRadians() const;

            /**
             * \return The Pitch in degrees.
             */
            float getPitch() const;

            /**
             * \return The Pitch in radians.
             */
            float getPitchRadians() const;

            /**
             * \return The Roll in degrees.
             */
            float getRoll() const;

            /**
             * \return The Roll in radians.
             */
            float getRollRadians() const;
            
            /**
             * Directly sets the yaw of this Euler angles. 
             * This method should only be used if you know exactly what you are doing.
             *
             * \param[in] yaw The Yaw in degrees.
             */
            void setYaw(float const yaw);

            /**
             * \param[in] yaw The Yaw in radians.
             */
            void setYawRadians(float const yaw);

            /**
             * Directly sets the pitch of this Euler angles. 
             * This method should only be used if you know exactly what you are doing.
             *
             * \param[in] pitch The Pitch in degrees.
             */
            void setPitch(float const pitch);

            /**
             * \param[in] pitch The Pitch in radians.
             */
            void setPitchRadians(float const pitch);

            /**
             * Directly sets the roll of this Euler angles. 
             * This method should only be used if you know exactly what you are doing.
             *
             * \param[in] roll The Roll in degrees.
             */
            void setRoll(float const roll);

            /**
             * \param[in] roll The Roll in radians.
             */
            void setRollRadians(float const roll);

            //------------------------------------------------------------
            // CONVERSIONS
            //------------------------------------------------------------
            
            /**
             * Converts this Euler angle representation of a rotation to
             * a 3x3 rotation matrix.
             *
             * \return The converted matrix.
             */
            Matrix3x3 toRotationMatrix() const;

            /**
             * Converts this Euler angle representation of a rotation to
             * a Quaternion.
             *
             * \return The converted quaternion.
             */
            Quaternion toQuaternion() const;

            //------------------------------------------------------------
            // OPERATIONS
            //------------------------------------------------------------

            /**
             * \param[in] angle
             * \return The angle in the range (-180, 180]
             */
            static float normalizeAxis(float const& angle);

            /**
             * \param[in] angle
             * \return The angle in the range [0, 360)
             */
            static float denormalizeAxis(float const& angle);

            /**
             * \note This method modifies the internal data stored in the vector. See getNormalized if this is not desired.
             */
            void normalize();

            /**
             *
             */
            Euler getNormalized() const;

            /**
             * \note This method modifies the internal data stored in the vector. See getNormalized if this is not desired.
             */
            void denormalize();

            /**
             *
             */
            Euler getDenormalized() const;

        protected:

        private:

            float m_Yaw;           ///< The Yaw. Stored internally as radians.
            float m_Pitch;         ///< The Pitch. Stored internally as radians.
            float m_Roll;          ///< The Roll. Stored internally as radians.
        };
    }
    /**
     * @} End of Doxygen Groups
     */
}
/**
 * @} End of Doxygen Groups
 */

OCULAR_REGISTER_TYPE_CUSTOM(Ocular::Math::Euler, "Euler");

//------------------------------------------------------------------------------------------

#endif