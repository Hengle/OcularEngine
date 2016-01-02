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

//------------------------------------------------------------------------------------------
// Default Constant Buffers
//------------------------------------------------------------------------------------------

/// Buffer that is updated on a per-frame basis
cbuffer cbPerFrame : register(b0)
{
    float4 _DummyData;                      ///< Placeholder data so that the buffer is not empty
};

/// Buffer that is updated on a per-camera basis (potentially multiple times each frame)
cbuffer cbPerCamera : register(b1)
{
    matrix _ViewMatrix;                     ///< The View Matrix of the current Camera
    matrix _ProjMatrix;                     ///< The Projection Matrix of the current Camera
    matrix _ViewProjMatrix;                 ///< The combined View-Projection Matrix
    float4 _EyePosition;                    ///< Position of the current Camera in World Space
};

/// Buffer that is updated on a per-SceneObject basis (potentially multiple times each camera)
cbuffer cbPerObject : register(b2)
{
    matrix _ModelMatrix;                    ///< The Model Matrix of the Object that owns the current geometry
};

// Any additional Constant Buffers (b3 and on) are shader/material dependent and are free to use

//------------------------------------------------------------------------------------------
// Default Structures
//------------------------------------------------------------------------------------------

/// Default Vertex Shader input structure. Based on: Ocular::Graphics::Vertex
struct VSInput
{
    float4 position : POSITION0;            ///< Position in model space of the incoming Vertex
    float4 color    : COLOR0;               ///< Default color of the incoming Vertex
    float3 normal   : NORMAL0;              ///< Normal of the incoming Vertex

    float2 uv0      : TEXCOORD0;            ///< 
    float2 uv1      : TEXCOORD1;            ///< 
    float2 uv2      : TEXCOORD2;            ///< 
    float2 uv3      : TEXCOORD3;            ///< 
};