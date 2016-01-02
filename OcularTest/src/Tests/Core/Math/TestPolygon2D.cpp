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

#include "Math/Geometry/Polygon2D.hpp"
#include "gtest/gtest.h"

using namespace Ocular::Math;

//------------------------------------------------------------------------------------------

TEST(Polygon2D, Centroid)
{
    std::vector<Point2f> points = 
    {
        Point2f(0.0f, 0.0f),
        Point2f(1.0f, 0.0f),
        Point2f(1.0f, 1.0f),
        Point2f(0.0f, 1.0f)
    };

    Polygon2D polygon(points);

    const Point2f expectedCentroid(0.5f, 0.5f);
    const float expectedArea = 1.0f;

    const Point2f calculatedCentroid = polygon.getCenter();
    const float calculatedArea = polygon.getSignedArea();

    EXPECT_TRUE(calculatedCentroid == expectedCentroid);
    EXPECT_NEAR(calculatedArea, expectedArea, EPSILON_FLOAT);
}