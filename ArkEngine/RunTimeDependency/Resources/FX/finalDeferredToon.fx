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
};

Texture2D PositionTexture : register(t0);
Texture2D DiffuseAlbedoTexture : register(t1);
Texture2D BumpedNormalTexture : register(t2);
Texture2D EmissiveTexture : register(t3);
Texture2D MaterialTexture : register(t5);

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

float4 ToonColorRamp[2] =
{
	// Ambient light
	// 0.0f로 하면 완전 검정색으로 출력된다
	float4(0.5f, 0.5f, 0.5f, 1.0f),  // 어두운 영역 색상(0.5 어둡게 출력)
	float4(1.0f, 1.0f, 1.0f, 1.0f)   // 하이라이트 영역 색상(원색을 출력)
};

float ToonShade(float intensity)
{
	//// 색상 런프를 기반으로 툰 쉐이딩 색상 결정
	//if (intensity < 0.1f) return ToonColorRamp[0];
	//else if (intensity < 0.5f) return ToonColorRamp[1];
	////else if (intensity < 0.75f) return ToonColorRamp[2];
	//else return ToonColorRamp[2];

	// 빛의 강함으로 음영질 부분을 구분한다
	if (intensity < 0.5f) return ToonColorRamp[0];
	else return ToonColorRamp[1];

}

void GetGBufferAttributes(float2 texCoord, out float3 normal, out float3 position, out float3 diffuseAlbedo, out float3 emissive, out float4 material)
{
	position = PositionTexture.Sample(samAnisotropic, texCoord).xyz;

	diffuseAlbedo = DiffuseAlbedoTexture.Sample(samAnisotropic, texCoord).xyz;

	normal = BumpedNormalTexture.Sample(samAnisotropic, texCoord).xyz;

	emissive = EmissiveTexture.Sample(samAnisotropic, texCoord).xyz;

	material = MaterialTexture.Sample(samAnisotropic, texCoord);
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
	float3 emissive;
	float4 material;

	float _Glossiness;
	float4 _SpecularColor;


	GetGBufferAttributes(pin.Tex, normal, position, diffuseAlbedo, emissive, material);

	float3 toEye = gEyePosW - position;

	float distToEye = length(toEye);

	toEye /= distToEye;

	float4 texColor = float4(diffuseAlbedo, 1.0f);

	float3 toLightDir = -gDirLights[0].Direction;

	// NdotL
	float NdotL = dot(normal, toLightDir);
	// 빛의 강도
	float lightIntensity = smoothstep(0, 0.01f, NdotL);
	// 빛의 강도에 따라 색상 출력의 레이어를 정해준다
	float4 toonColor = ToonShade(lightIntensity);
	float4 litColor = texColor * toonColor;

	// Specular Reflection
	_Glossiness = 32.f;
	_SpecularColor = (0.9f, 0.9f, 0.9f, 1.f);

	float3 halfVector = normalize(toLightDir + toEye);
	float NdotH = dot(normal, halfVector);
	float specularIntensity = pow(NdotH * lightIntensity, _Glossiness * _Glossiness);
	float specularIntensitySmooth = smoothstep(0.005, 0.01, specularIntensity);
	float4 specular = specularIntensitySmooth * _SpecularColor;


	// Emissive
	float4 emissiveColor = float4(emissive, 1.0f);
	litColor += emissiveColor;

	// OutLine
	float outlineThreshold = 0.9f; // 외곽선 감지 임계값
	float3 outlineColor = float3(0.0f, 0.0f, 0.0f); // 외곽선 색상
	float outlineFactor = saturate(1.0f - dot(normal, toLightDir)); // 노멀과 dirLight 간의 각도에 따라 외곽선 감도 결정
	float4 outline = outlineFactor > outlineThreshold ? float4(outlineColor, 1.0f) : float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	// 원래 물체의 색상과 외곽선을 더함
	float4 finalColor = litColor + outline + specular;


	 return finalColor;
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