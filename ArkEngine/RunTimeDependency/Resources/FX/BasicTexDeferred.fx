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

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;
TextureCube gCubeMap;

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
};

struct PSOut
{
    float4 Position : SV_Target0;
    float4 Diffuse : SV_Target1;
    float4 BumpedNormal : SV_Target2;
    float4 Depth : SV_Target3;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
	
	// Transform to world space space.
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose);
    vout.TangentW = mul(vin.TangentL, (float3x3) gWorld);
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
	//vout.Tex = vin.Tex;


    return vout;
}
 
PSOut PS(VertexOut pin, uniform bool gUseTexure, uniform bool gReflect)
{
    PSOut output;
    
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);
    
    float3 normalMap = gNormalMap.Sample(samAnisotropic, pin.Tex).xyz;
    
    float3 diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex).xyz;
    
    float4 orthonormalizedTangent;
    
    float4 bumpedNormal = NormalSampleToWorldSpace(normalMap, pin.NormalW, pin.TangentW, orthonormalizedTangent);
    
    output.Position = float4(pin.PosW, 1.0f);
    output.Diffuse = float4(diffuse, 1.0f);
    output.BumpedNormal = bumpedNormal;
    output.Depth = float4(pin.PosH.zzz, 1.0f);
    
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