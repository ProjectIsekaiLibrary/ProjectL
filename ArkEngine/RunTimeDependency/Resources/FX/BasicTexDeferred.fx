
//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

cbuffer cbPerObject
{
    float4x4 gWorld[100];
    float4x4 gWorldInvTranspose[100];
    float4x4 gWorldViewProj[100];
    float4x4 gTexTransform;
    Material gMaterial;
    
        // Dissolve
    float gDissolveValue;
    float gGradation;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;
Texture2D gEmissiveMap;
Texture2D gMaskMap;

float gCartoon;

// Dissolve Effect
Texture2D gNoiseTexture;
Texture2D gBurnTexture;

float4 gColor[100];

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentW : TANGENT;
    uint InstanceID : BLENDINDICES;
};

struct PSOut
{
    float4 Position : SV_Target0;
    float4 Diffuse : SV_Target1;
    float4 BumpedNormal : SV_Target2;
    float4 Emissive : SV_Target3;
    float4 Material : SV_Target4;
    float4 Additional : SV_Target5;
    float4 Color : SV_Target6;
};

struct PSOut2
{
    float4 Color : SV_Target0;
};

VertexOut VS(VertexIn vin, uint instanceID : SV_InstanceID)
{
    VertexOut vout;

    // Transform to world space space.
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld[instanceID]).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose[instanceID]);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld[instanceID]);
    // Transform to homogeneous clip space.    
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj[instanceID]);

    // Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    //vout.Tex = vin.Tex;
    vout.InstanceID = instanceID;

    return vout;
}

PSOut PS(VertexOut pin, uniform bool gUseTexure, uniform bool gReflect)
{
    PSOut output;

    // Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

    float3 normalMap = gNormalMap.Sample(samAnisotropic, pin.Tex).xyz;

    float3 diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex).xyz;

    float3 emissive = gMaskMap.Sample(samAnisotropic, pin.Tex).xyz;

    float4 orthonormalizedTangent;

    float4 bumpedNormal = NormalSampleToWorldSpace(normalMap, pin.NormalW, pin.TangentW, orthonormalizedTangent);
    
    output.Position = float4(pin.PosW, 1.0f);
    output.Diffuse = float4(diffuse, 1.0f);
    output.BumpedNormal = bumpedNormal;
    output.Emissive = float4(emissive, 1.0f);
    output.Material = float4(gMaterial.Ambient.x, gMaterial.Diffuse.x, gMaterial.Specular.x, gMaterial.Specular.w);
    output.Additional = float4(gCartoon, 0.0f, 0.0f, 1.0f);
    output.Color = gColor[pin.InstanceID];

    //    // 디졸브 효과 계산
    //float dissolveFactor = gDissolveValue; // 디졸브 효과에 사용될 값으로, 예를 들어 uniform으로 설정된 값으로 계산할 수 있습니다.
    //float noiseVel = gNoiseTexture.Sample(samAnisotropic, pin.Tex).x; // 노이즈 텍스처 샘플링
    //float dissolveSmooth = smoothstep(0.0f, 1.0f, dissolveFactor + noiseVel); // 디졸브 효과를 부드럽게 만듭니다.

//    //
    //// 디졸브 효과를 디졸브 효과에 적용할 렌더 타깃에 곱합니다.
    //output.Diffuse *= dissolveSmooth;
    //output.BumpedNormal *= dissolveSmooth;
    //output.Emissive *= dissolveSmooth;
    //output.Material *= dissolveSmooth;
    //output.Color *= dissolveSmooth;
    
    return output;
}

PSOut2 PS2(VertexOut pin)
{
    PSOut2 output;
    output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return output;
}

technique11 Light
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(false, false)));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS2()));
    }
}

technique11 LightTex
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(true, false)));
    }
}

technique11 LightTexReflect
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(true, true)));
    }
}