//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    float4x4 gTexTransform;
    Material gMaterial;
};

cbuffer cbSkinned
{
    float4x4 gBoneTransforms[250];
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;
Texture2D gEmissiveMap;


TextureCube gCubeMap;
float4 gColor;
float gCartoon;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

// Ãß°¡
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;

    float3 Weight : BLENDWEIGHT;

    uint4 BoneIndicles : BLENDINDICES;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD0;
    float3 TangentW : TANGENT;
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

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = vin.Weight.x;
    weights[1] = vin.Weight.y;
    weights[2] = vin.Weight.z;
    weights[3] = 1 - weights[0] - weights[1] - weights[2];

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);

    posL += weights[0] * mul(float4(vin.PosL, 1.0F), gBoneTransforms[vin.BoneIndicles.x]).xyz;
    normalL += weights[0] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndicles.x]);

    posL += weights[1] * mul(float4(vin.PosL, 1.0F), gBoneTransforms[vin.BoneIndicles.y]).xyz;
    normalL += weights[1] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndicles.y]);

    posL += weights[2] * mul(float4(vin.PosL, 1.0F), gBoneTransforms[vin.BoneIndicles.z]).xyz;
    normalL += weights[2] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndicles.z]);

    posL += weights[3] * mul(float4(vin.PosL, 1.0F), gBoneTransforms[vin.BoneIndicles.w]).xyz;
    normalL += weights[3] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndicles.w]);


	// Transform to world space space.
    vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(normalL, (float3x3) gWorldInvTranspose);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);
    
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);

	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	///vout.Tex = vin.Tex;
    
    //float4x4 lightViewMatrix = gLightViewMatrix;
    //float3 dirZ = -normalize(gWorldLightPosition.xyz);
    //float3 up = float3(0.0f, 1.0f, 0.0f);
    //float3 dirX = cross(up, dirZ);
    //float3 dirY = cross(dirZ, dirX);
    //
    //lightViewMatrix = float4x4(
    //float4(dirX, -dot(gWorldLightPosition.xyz, dirX)),
    //float4(dirY, -dot(gWorldLightPosition.xyz, dirY)),
    //float4(dirZ, -dot(gWorldLightPosition.xyz, dirZ)),
    //float4(0.0f, 0.0f, 0.0f, 1.0f));
    //
    //lightViewMatrix = transpose(lightViewMatrix);
    
    
    return vout;
}

PSOut PS(VertexOut pin, uniform bool gUseTexure, uniform bool gReflect)
{
    PSOut output;

    // Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

    float3 normalMap = gNormalMap.Sample(samAnisotropic, pin.Tex).xyz;

    float3 diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex).xyz;

    float3 emissive = gEmissiveMap.Sample(samAnisotropic, pin.Tex).xyz;

    float4 orthonormalizedTangent;

    float4 bumpedNormal = NormalSampleToWorldSpace(normalMap, pin.NormalW, pin.TangentW, orthonormalizedTangent);

    output.Position = float4(pin.PosW, 1.0f);
    output.Diffuse = float4(diffuse, 1.0f);
    output.BumpedNormal = bumpedNormal;
    output.Emissive = float4(emissive, 1.0f);
    output.Material = float4(gMaterial.Ambient.x, gMaterial.Diffuse.x, gMaterial.Specular.x, gMaterial.Specular.w);
    output.Additional = float4(gCartoon, 0.0f, 0.0f, 1.0f);
    output.Color = gColor;
    
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