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
	//Texture2D gShadowMap;
};

cbuffer cbSkinned
{
	float4x4 gBoneTransforms[150];
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;
Texture2D gEmissiveMap;

TextureCube gCubeMap;
float4 gColor;

//SamplerComparisonState samShadowMap
//{
//	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//	ComparisonFunc = LESS_EQUAL;	// 그림자 맵 샘플링에 사용되는 비교 함수
//};

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

	float3 Weight : BLENDWEIGHT;

	uint4 BoneIndicles : BLENDINDICES;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
	float3 TangentW : TANGENT;
};


struct ToonPSOut
{
	float4 Position : SV_Target0;
	float4 Diffuse : SV_Target1;
	float4 BumpedNormal : SV_Target2;
	float4 Emissive : SV_Target3;
	float4 Depth : SV_Target4;
	float4 Material : SV_Target5;
	float4 Color : SV_Target6;
	//float4 Outline : SV_Target5;    // 경게선
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


	return vout;
}


ToonPSOut ToonPS(VertexOut pin, uniform bool gUseTexture, uniform bool rReflect)
{
	ToonPSOut output;

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
	output.Depth = float4(pin.PosH.zzz, 1.0f);
	output.Material = float4(gMaterial.Ambient.x, gMaterial.Diffuse.x, gMaterial.Specular.x, gMaterial.Specular.w);
	output.Color = gColor;

	// Toon Shading
	float toonThreshold = 0.5f;     // 경계선을 결정할 임계값
	float toonOutline = 0.02f;      // 경계선의 두께

	// NdotL
	float ndotl = dot(pin.NormalW, float3(0.0, 0.0, 0.0)); // 빛의 방향에 따른 노멀 값

	//float halfLambert = ndotl * 0.5 + 0.5;
	//output.Diffuse = ceil(halfLambert * 3) * (1/3); // 낮은 빛의 강도에서 색상 구분

	output.Diffuse = ceil(output.Diffuse * 6.0) / 6.0; // 낮은 빛의 강도에서 색상 구분

	// 툰셰이딩의 경계선을 생성
	float outline = fwidth(ndotl * 100.0);  // 빛의 방향에 따른 경계값 계산
	//output.Outline = step(outline - toonOutline, toonOutline);

	// 경계선을 알파 채널에 적용
	//output.Diffuse.a *= output.Outline;


	// 그림자 계산
	//float shadow = gShadowMap.SampleCmpLevelZero(samShadowMap, float2(pin.PosH.x / pin.PosH.w, pin.PosH.y / pin.PosH.w), pin.PosH.z / pin.Posh.w).r;
	
	// 그림자를 사용하여 조명 계산 조정
	//output.Diffuse.rgb *= shadow;

	return output;
}


technique11 Toonlight
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ToonPS(false, false)));
	}
};

// Toon 셰이딩에 텍스쳐를 사용하는 버전
technique11 ToonLightTex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ToonPS(true, false)));
	}
}

// Toon 셰이딩에 텍스쳐와 반사 효과를 사용하는 버전
technique11 ToonLightTexReflect
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ToonPS(true, true)));
	}
}
