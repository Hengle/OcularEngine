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

#include "Math/Bounds/BoundsAABB.hpp"
#include "Math/Bounds/BoundsSphere.hpp"
#include "Math/Bounds/BoundsOBB.hpp"
#include "Math/Bounds/Ray.hpp"
#include "Math/Geometry/Plane.hpp"

//------------------------------------------------------------------------------------------

namespace Ocular
{
    namespace Math
    {
        //----------------------------------------------------------------------------------
        // CONSTRUCTORS
        //----------------------------------------------------------------------------------

        BoundsAABB::BoundsAABB(std::list<Point3f> const& points)
        {
            construct(points);
        }

        BoundsAABB::BoundsAABB(Vector3f const& center, Vector3f const& extents)
        {
            m_Center  = center;
            m_Extents = Vector3f(fmaxf(extents.x, 0.0f), fmaxf(extents.y, 0.0f), fmaxf(extents.z, 0.0f));

            calculateMinMaxPoints();
        }

        BoundsAABB::BoundsAABB()
        {
        
        }

        BoundsAABB::~BoundsAABB()
        {
        
        }

        //----------------------------------------------------------------------------------
        // PUBLIC METHODS
        //----------------------------------------------------------------------------------

        void BoundsAABB::construct(std::list<Point3f> const& points)
        {
            float minX = FLT_MAX;
            float minY = FLT_MAX;
            float minZ = FLT_MAX;

            float maxX = FLT_MIN;
            float maxY = FLT_MIN;
            float maxZ = FLT_MIN;

            for(auto point : points)
            {
                minX = fminf(minX, point.x);
                minY = fminf(minY, point.y);
                minZ = fminf(minZ, point.z);

                maxX = fmaxf(maxX, point.x);
                maxY = fmaxf(maxY, point.y);
                maxZ = fmaxf(maxZ, point.z);
            }

            m_MinPoint = Vector3f(minX, minY, minZ);
            m_MaxPoint = Vector3f(maxX, maxY, maxZ);
            m_Center   = Vector3f::Midpoint(m_MinPoint, m_MaxPoint);
            m_Extents  = m_MaxPoint - m_Center;
        }

        void BoundsAABB::setCenter(Vector3f const& center)
        {
            m_Center = center;
            calculateMinMaxPoints();
        }

        void BoundsAABB::setExtents(Vector3f const& extents)
        {
            m_Extents = Vector3f(fmaxf(extents.x, 0.0f), fmaxf(extents.y, 0.0f), fmaxf(extents.z, 0.0f));
            calculateMinMaxPoints();
        }

        Vector3f const& BoundsAABB::getCenter() const
        {
            return m_Center;
        }

        Vector3f const& BoundsAABB::getExtents() const
        {
            return m_Extents;
        }

        Vector3f const& BoundsAABB::getMinPoint() const
        {
            return m_MinPoint;
        }

        Vector3f const& BoundsAABB::getMaxPoint() const
        {
            return m_MaxPoint;
        }

        void BoundsAABB::expand(Vector3f const& extents)
        {
            m_Extents += extents;

            m_Extents.x = fmaxf(m_Extents.x, 0.0f);
            m_Extents.y = fmaxf(m_Extents.y, 0.0f);
            m_Extents.z = fmaxf(m_Extents.z, 0.0f);

            calculateMinMaxPoints();
        }

        void BoundsAABB::expandToContain(Vector3f const& point)
        {
            if(!contains(point))
            {
                m_MinPoint = Vector3f(fminf(m_MinPoint.x, point.x),
                                      fminf(m_MinPoint.y, point.y),
                                      fminf(m_MinPoint.z, point.z));

                m_MaxPoint = Vector3f(fmaxf(m_MaxPoint.x, point.x),
                                      fmaxf(m_MaxPoint.y, point.y),
                                      fmaxf(m_MaxPoint.z, point.z));

                m_Center  = Vector3f::Midpoint(m_MinPoint, m_MaxPoint);
                m_Extents = m_MaxPoint - m_Center;
            }
        }

        void BoundsAABB::expandToContain(BoundsAABB const& bounds)
        {
            IntersectionType result;
            contains(bounds, &result);

            if(result != IntersectionType::Inside)
            {
                const Vector3f otherMin = bounds.getMinPoint();
                const Vector3f otherMax = bounds.getMaxPoint();

                m_MinPoint = Vector3f(fminf(m_MinPoint.x, otherMin.x),
                                      fminf(m_MinPoint.y, otherMin.y),
                                      fminf(m_MinPoint.z, otherMin.z));

                m_MaxPoint = Vector3f(fmaxf(m_MaxPoint.x, otherMax.x),
                                      fmaxf(m_MaxPoint.y, otherMax.y),
                                      fmaxf(m_MaxPoint.z, otherMax.z));

                m_Center  = Vector3f::Midpoint(m_MinPoint, m_MaxPoint);
                m_Extents = m_MaxPoint - m_Center;
            }
        }

        //------------------------------------------------------------------------------
        // Intersection and Containment Testing
        //------------------------------------------------------------------------------

        bool BoundsAABB::intersects(Ray const& ray) const
        {
            return ray.intersects((*this));
        }

        bool BoundsAABB::intersects(Ray const& ray, Point3f& point, float& distance) const
        {
            return ray.intersects((*this), point, distance);
        }

        bool BoundsAABB::intersects(BoundsSphere const& bounds) const
        {
            return bounds.intersects((*this));
        }

        bool BoundsAABB::intersects(BoundsAABB const& bounds) const
        {
            // Source: Real-Time Rendering, 3rd Ed. Page 765

            const Vector3f minB = bounds.getMinPoint();
            const Vector3f maxB = bounds.getMaxPoint();

            return !((m_MinPoint.x > maxB.x) || (minB.x > m_MaxPoint.x) ||
                     (m_MinPoint.y > maxB.y) || (minB.y > m_MaxPoint.y) ||
                     (m_MinPoint.z > maxB.z) || (minB.z > m_MaxPoint.z));
        }

        bool BoundsAABB::intersects(BoundsOBB const& bounds) const
        {
            return false;
        }

        bool BoundsAABB::intersects(Plane const& plane, IntersectionType* result) const
        {
            // Source: Real-Time Rendering, 3rd Ed. Page 756
          
            bool intersects = true;
            IntersectionType tempResult = IntersectionType::Intersects;

            const Vector3f& planeNormal  = plane.getNormal();

            const float extent = (m_Extents.x * fabsf(planeNormal.x)) + (m_Extents.y * fabsf(planeNormal.y)) + (m_Extents.z * fabsf(planeNormal.z));
            const float signedDistance = m_Center.dot(planeNormal) + (plane.getPoint().dot(plane.getPoint()));

            if((signedDistance - extent) > 0.0f)
            {
                intersects = false;
                tempResult = IntersectionType::Outside;
            }
            else if((signedDistance + extent) < 0.0f)
            {
                intersects = false;
                tempResult = IntersectionType::Inside;
            }

            if(result)
            {
                (*result) = tempResult;
            }

            return intersects;
        }

        bool BoundsAABB::contains(Vector3f const& point, IntersectionType* result) const
        {
            IntersectionType tempResult;

            // If just any one of the components lies beyond the min/max points, then it is outside.
            if((point.x > m_MaxPoint.x) || (point.y > m_MaxPoint.y) || (point.z > m_MaxPoint.z) ||
               (point.x < m_MinPoint.x) || (point.y < m_MinPoint.y) || (point.z < m_MinPoint.z))
            {
                tempResult = IntersectionType::Outside;
            }
            else
            {
                tempResult = IntersectionType::Inside;

                if(IsEqual<float>(point.x, m_MaxPoint.x) || IsEqual<float>(point.y, m_MaxPoint.y) || IsEqual<float>(point.z, m_MaxPoint.z) ||
                    IsEqual<float>(point.x, m_MinPoint.x) || IsEqual<float>(point.y, m_MinPoint.y) || IsEqual<float>(point.z, m_MinPoint.z))
                {
                    tempResult = IntersectionType::Intersects;
                }
            }

            if(result)
            {
                (*result) = tempResult;
            }

            return (tempResult == IntersectionType::Outside) ? false : true;  // Avoid MSVC compiler warning from casting direct to bool
        }

        bool BoundsAABB::contains(BoundsAABB const& bounds, IntersectionType* result) const
        {
            IntersectionType tempResult;

            const Vector3f otherMin = bounds.getMinPoint();
            const Vector3f otherMax = bounds.getMaxPoint();

            if((otherMin > m_MaxPoint) || (otherMax < m_MinPoint))
            {
                tempResult = IntersectionType::Outside;
            }
            else
            {
                tempResult = IntersectionType::Inside;

                if((otherMax.x >= m_MaxPoint.x) || (otherMax.y >= m_MaxPoint.y) || (otherMax.z >= m_MaxPoint.z) ||
                    (otherMin.x <= m_MinPoint.x) || (otherMin.y <= m_MinPoint.y) || (otherMin.z <= m_MinPoint.z))
                {
                    tempResult = IntersectionType::Intersects;
                }
            }

            if(result)
            {
                (*result) = tempResult;
            }
            
            return (tempResult == IntersectionType::Outside) ? false : true;  // Avoid MSVC compiler warning from casting direct to bool
        }

        //----------------------------------------------------------------------------------
        // PROTECTED METHODS
        //----------------------------------------------------------------------------------

        void BoundsAABB::calculateMinMaxPoints()
        {
            m_MinPoint = m_Center - m_Extents;
            m_MaxPoint = m_Center + m_Extents;
        }

        //----------------------------------------------------------------------------------
        // PRIVATE METHODS
        //----------------------------------------------------------------------------------
    }
}