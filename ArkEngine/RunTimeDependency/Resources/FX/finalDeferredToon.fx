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
    
    float4x4 gLightView;
    float4x4 gLightProj;
};

// ���� �޴� ��ä�� ������
Texture2D PositionTexture : register(t0);
Texture2D DiffuseAlbedoTexture : register(t1);
Texture2D BumpedNormalTexture : register(t2);
Texture2D EmissiveTexture : register(t3);
Texture2D MaterialTexture : register(t4);
Texture2D AdditionalTexture : register(t5);
// ���� �� ���޿�(G-Buffer�� ����� depth ������ �����ͼ� �׸���)
Texture2D gShadowDepthMapTexture;

TextureCube gCubeMap;

SamplerState samAnisotropic
{
    Filter = MIN_MAG_MIP_LINEAR;

	//MaxAnisotropy = 4;

    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
};

SamplerComparisonState samShadow
{
    Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComparisonFunc = LESS_EQUAL;
};

struct VertexIn
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};


float4 ToonColorRamp[2] =
{
	// Ambient light
	// 0.0f�� �ϸ� ���� ���������� ��µȴ�
    float4(0.4f, 0.4f, 0.4f, 1.0f), // ��ο� ���� ����(0.5 ��Ӱ� ���)
	float4(1.0f, 1.0f, 1.0f, 1.0f) // ���̶���Ʈ ���� ����(������ ���)
};

float ToonShade(float intensity)
{
	//// ���� ������ ������� �� ���̵� ���� ����
	//if (intensity < 0.1f) return ToonColorRamp[0];
	//else if (intensity < 0.5f) return ToonColorRamp[1];
	////else if (intensity < 0.75f) return ToonColorRamp[2];
	//else return ToonColorRamp[2];

	// ���� �������� ������ �κ��� �����Ѵ�
    if (intensity < 0.5f)
        return ToonColorRamp[0];
    else
        return ToonColorRamp[1];

}


void GetGBufferAttributes(float2 texCoord, out float3 normal, out float3 position, out float3 diffuseAlbedo, out float3 emissive, out float4 material, out float cartoon)
{
    position = PositionTexture.Sample(samAnisotropic, texCoord).xyz;

    diffuseAlbedo = DiffuseAlbedoTexture.Sample(samAnisotropic, texCoord).xyz;

    normal = BumpedNormalTexture.Sample(samAnisotropic, texCoord).xyz;
	
    emissive = EmissiveTexture.Sample(samAnisotropic, texCoord).xyz;
    
    material = MaterialTexture.Sample(samAnisotropic, texCoord);
    
    cartoon = AdditionalTexture.Sample(samAnisotropic, texCoord).x;
}

float4 WorldToLight(float3 position)
{
    float4 worldPos = float4(position, 1.0f);
    
    float4 worldPosByLight = mul(worldPos, gLightView);
    
    worldPosByLight = mul(worldPosByLight, gLightProj);
    
    return worldPosByLight;
}

float2 ToTexcoord(float4 position)
{
    float4 ndcPos = position / position.w;
    
    float2 texCoord = float2((ndcPos.x + 1.0f) * 0.5f, (1.0f - ndcPos.y) * 0.5f);
    
    return texCoord;
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
    float cartoon;
    
	// Specular Reflaction
    float _Glossiness;
    float4 _SpecularColor;

    GetGBufferAttributes(pin.Tex, normal, position, diffuseAlbedo, emissive, material, cartoon);
  
    float3 toEye = gEyePosW - position;

    float distToEye = length(toEye);

    toEye /= distToEye;

    // ���� �ȼ��� ������ǥ�� ������ VIEW,RPOJ����� ���� ������������ �̵���Ŵ
    float4 worldPosByLight = WorldToLight(position);
    
    // ���� �ȼ��� �������������� Z���� ������
    float vertexDepth = worldPosByLight.z / worldPosByLight.w;
    // ������������ ������Ʈ�� ���̸� ��Ƴ��� �ؽ��Ŀ��� ���� �ȼ��� Z���� ������
    float lightDepth = gShadowDepthMapTexture.Sample(samAnisotropic, ToTexcoord(worldPosByLight)).r;
    
    float shadowFactor;
    
    // ���� �տ� �ٸ� ��ü�� �־� LIGHTDepth�� �� �۴ٸ� �׸��ڰ� ����
    if (vertexDepth > lightDepth + 0.0000125f)
    {
        shadowFactor = 0.5f;
    }
    else
    {
        shadowFactor = 1.0f;
    }
    
    float4 texColor = float4(diffuseAlbedo * shadowFactor, 1.0f);

    // Start with a sum of zero. 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    
    Material nowMat;
    nowMat.Ambient = float4(material.x, material.x, material.x, 1.0f);
    nowMat.Diffuse = float4(material.y, material.y, material.y, 1.0f);
    nowMat.Specular = float4(material.z, material.z, material.z, material.w);
    nowMat.Reflect = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float3 toLightDir = -gDirLights[0].Direction;


   
	// Rim Lighting
    float rimIntensity = 0.2f;
    float rimThreshold = 0.2f;
    float rim = saturate(dot(normal, toLightDir)); // �ü� ������ �������� Rim�� ����մϴ�
    rim = smoothstep(1.0f - rimThreshold, 1.0f, rim); // �Ӱ谪�� ���� Rim�� �ε巴�� �����մϴ�
    float3 rimColor = float3(1.0f, 0.0f, 0.0f); // Rim ���� ������ �����մϴ� (�ʿ信 ���� ���� ����)
    float4 rimLighting = rim * rimIntensity * float4(rimColor, 1.0f);

	// Toon Shading
	// NdotL
    //float NdotL = dot((texColor,1.0f), toLightDir);
    float NdotL = dot(normal, toLightDir);

    float4 litColor;
    float lightIntensity;
    float4 toonColor;
    
    if (cartoon == 1.0f)
    {
       	// ���� ����
        lightIntensity = smoothstep(0, 1.f, NdotL);
    }
    else
    {
        // ���� ����
        lightIntensity = smoothstep(1.f, 1.f, NdotL);
    }
    
    //float lightIntensity = NdotL > 0 ? 1 : 0;
	// ���� ������ ���� ���� ����� ���̾ �����ش� 
    toonColor = ToonShade(lightIntensity);
    
    litColor = texColor * toonColor;
    
	// Specular Reflection
    _Glossiness = 32.f;
    _SpecularColor = (0.0f, 0.0f, 0.0f, 1.f);
    
	//float3 halfVector = normalize(toLightDir + toEye);
    float3 halfVector = normalize(toLightDir);
    float NdotH = dot(normal, halfVector);
    float specularIntensity = pow(NdotH * lightIntensity, _Glossiness * _Glossiness);
    float specularIntensitySmooth = smoothstep(0.005, 0.01, specularIntensity);
    float4 specular = specularIntensitySmooth * _SpecularColor;

	// Emissive
    float4 emissiveColor = float4(emissive, 1.0f);
    litColor += emissiveColor;

	// OutLine
    float outlineThreshold = 0.99f; // �ܰ��� ���� �Ӱ谪
    float4 outlineColor = float4(0.0f, 0.0f, 0.0f, 1.0f); // �ܰ��� ����
    float outlineFactor = saturate(1.0f - dot(normal, toLightDir)); // ��ְ� dirLight ���� ������ ���� �ܰ��� ���� ����
    float4 outline = outlineFactor > outlineThreshold ? outlineColor : float4(0.0f, 0.0f, 0.0f, 0.0f);

	
	// ���� ��ü�� ����� �ܰ����� ����
    float4 finalColor = litColor + outline + specular + rimLighting;

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