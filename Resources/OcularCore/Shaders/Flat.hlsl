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

 #include "OcularCommon.hlsl"

//------------------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------------------

cbuffer cbPerMaterial : register(b3)
{
    float4 _Color;
};

struct VSOutput
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
};

struct PSOutput
{
    float4 color : SV_Target;
};

//------------------------------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------------------------------

VSOutput VSMain(VSInput input)
{
    matrix mvpMatrix = mul(_ModelMatrix, _ViewProjMatrix);

    VSOutput output;

    output.position = mul(input.position, mvpMatrix);
    output.color    = _Color;

    return output;
}

//------------------------------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------------------------------

float4 PSMain(VSOutput input) : SV_Target
{
    return input.color;
}