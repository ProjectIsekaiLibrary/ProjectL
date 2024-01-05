//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
    int gDirLightCount;
    DirectionalLight gDirLights[3];
    
    int gPointLightCount;
    PointLight gPointLights[10];
    
    float3 gEyePosW;
    
    Material gMaterial;
};

Texture2D PositionTexture : register(t0);
Texture2D DiffuseAlbedoTexture : register(t1);
Texture2D BumpedNormalTexture : register(t2);
Texture2D DepthTexture : register(t3);

TextureCube gCubeMap;

SamplerState samAnisotropic
{
    Filter = MIN_MAG_MIP_LINEAR;

	//MaxAnisotropy = 4;

    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
};

struct VertexIn
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

void GetGBufferAttributes(float2 texCoord, out float3 normal, out float3 position, out float3 diffuseAlbedo)
{
    position = PositionTexture.Sample(samAnisotropic, texCoord).xyz;
    
    diffuseAlbedo = DiffuseAlbedoTexture.Sample(samAnisotropic, texCoord).xyz;
    
    normal = BumpedNormalTexture.Sample(samAnisotropic, texCoord).xyz;
}

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    vout.Pos = float4(vin.Pos, 1.0f);
    vout.Tex = vin.Tex;

    return vout;
}
 
float4 PS(VertexOut pin, uniform bool gUseTexure, uniform bool gReflect) : SV_Target
{
    float3 normal;
    float3 position;
    float3 diffuseAlbedo;
    
    GetGBufferAttributes(pin.Tex, normal, position, diffuseAlbedo);
    
    float3 toEye = gEyePosW - position;
    
    float distToEye = length(toEye);
    
    toEye /= distToEye;
    
    float4 texColor = float4(diffuseAlbedo, 1.0f);
    
    float4 litColor = texColor;
    
    // Start with a sum of zero. 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if (gDirLightCount > 0)
    {
		// Sum the light contribution from each light source.  
		[unroll]
        for (int i = 0; i < gDirLightCount; ++i)
        {
            float4 A, D, S;
        
            ComputeDirectionalLight(gMaterial, gDirLights[i], normal, toEye,
				A, D, S);
        
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }
    
    if (gPointLightCount > 0)
    {
        for (int j = 0; j < gPointLightCount; ++j)
        {
            float4 A, D, S;
        
            ComputePointLight(gMaterial, gPointLights[j], position, normal, toEye,
				A, D, S);
        
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }
    
	// Modulate with late add.
    litColor = texColor * (ambient + diffuse) + spec;

    return litColor;
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