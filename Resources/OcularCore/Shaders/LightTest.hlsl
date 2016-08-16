/**
 * Copyright 2014-2016 Steven T Sell (ssell@vertexfragment.com)
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

 #include "OcularLighting.hlsl"

//------------------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------------------

struct VSOutput
{
    float4 position : SV_Position;
    float4 color    : COLOR0;
    float4 normal   : NORMAL0;
    float4 uv0      : TEXCOORD0;
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
    output.normal   = mul(input.normal, mvpMatrix);
    output.color    = input.color;
    output.uv0      = input.uv0;

    return output;
}

//------------------------------------------------------------------------------------------
// Pixel Shader
//------------------------------------------------------------------------------------------

PSOutput PSMain(VSOutput input)
{
    PSOutput output;
    output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);

    //--------------------------------------------------------------------
    // Calculate lighting
    //--------------------------------------------------------------------

    const float4 toView = normalize(_EyePosition - input.position);
    const float4 ambient = _LightBuffer[0].color * _LightBuffer[0].parameters.x;

    float4 radiance = float4(0.0f, 0.0f, 0.0f, 1.0f);

    // Prepare for loop

    uint lightsCount = 0;
    uint lightsSize = 0;

    _LightBuffer.GetDimensions(lightsCount, lightsSize);

    // Loop over each dynamic light 

    [loop]
    for(uint i = 1; i < lightsCount; i++)
    {
        const float4 toLight = normalize(_LightBuffer[i].position - input.position);
        const float4 brdf    = phongBRDF(input.normal, toLight, toView, float4(1.0f, 1.0f, 1.0f, 1.0f), float4(0.1f, 0.1, 0.1, 1.0f), 4.0f);
        const float4 light   = _LightBuffer[i].color * _LightBuffer[i].parameters.x;

        radiance += brdf * light * ccosAngle(input.normal, toLight);
    }

    output.color = ambient + radiance;

    //--------------------------------------------------------------------

    return output;
}