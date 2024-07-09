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
    
    float gAttenuation;
    
    float4x4 gDecalWorldInv[10];
};

// 빛을 받는 객채의 포지션
Texture2D PositionTexture : register(t0);
Texture2D DiffuseAlbedoTexture : register(t1);
Texture2D BumpedNormalTexture : register(t2);
Texture2D EmissiveTexture : register(t3);
Texture2D MaterialTexture : register(t4);
Texture2D AdditionalTexture : register(t5);
// 깊이 값 전달용(G-Buffer에 저장된 depth 정보를 가져와서 그림자)
Texture2D gShadowDepthMapTexture;

TextureCube gCubeMap;

//SamplerState samAnisotropic
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//
//	//MaxAnisotropy = 4;
//
//    AddressU = CLAMP;
//    AddressV = CLAMP;
//    AddressW = CLAMP;
//};

float3 ToneMapReinhard(float3 color)
{
    float3 mappedColor = color / (color + float3(1.0, 1.0, 1.0));
    return mappedColor;
}

SamplerState samAnisotropic
{
    Filter = MIN_MAG_MIP_POINT;
    MaxAnisotropy = 16;

    AddressU = WRAP;
    AddressV = WRAP;

    MipLODBias = 0.0;
    MinLOD = 0;
    MaxLOD = 15;
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
	// 0.0f로 하면 완전 검정색으로 출력된다
    float4(0.4f, 0.4f, 0.4f, 1.0f), // 어두운 영역 색상(0.5 어둡게 출력)
	float4(1.0f, 1.0f, 1.0f, 1.0f) // 하이라이트 영역 색상(원색을 출력)
};

float ToonShade(float intensity)
{
	//// 색상 런프를 기반으로 툰 쉐이딩 색상 결정
	//if (intensity < 0.1f) return ToonColorRamp[0];
	//else if (intensity < 0.5f) return ToonColorRamp[1];
	////else if (intensity < 0.75f) return ToonColorRamp[2];
	//else return ToonColorRamp[2];

	// 빛의 강함으로 음영질 부분을 구분한다
    if (intensity < 0.5f)
        return ToonColorRamp[0];
    else
        return ToonColorRamp[1];

}


void GetGBufferAttributes(float2 texCoord, out float3 normal, out float3 position, out float4 diffuseAlbedo, out float3 emissive, out float4 material, out float cartoon, out float2 decal)
{
    position = PositionTexture.Sample(samAnisotropic, texCoord).xyz;

    diffuseAlbedo = DiffuseAlbedoTexture.Sample(samAnisotropic, texCoord);

    normal = BumpedNormalTexture.Sample(samAnisotropic, texCoord).xyz;
	
    emissive = EmissiveTexture.Sample(samAnisotropic, texCoord).xyz;
    
    material = MaterialTexture.Sample(samAnisotropic, texCoord);
    
    cartoon = AdditionalTexture.Sample(samAnisotropic, texCoord).x;
    
    decal = AdditionalTexture.Sample(samAnisotropic, texCoord).yz;
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
    float4 diffuseAlbedo;
    float3 emissive;
    float4 material;
    float cartoon;
    float2 decalInfo;
    
	// Specular Reflaction
    float _Glossiness;
    float4 _SpecularColor;

    GetGBufferAttributes(pin.Tex, normal, position, diffuseAlbedo, emissive, material, cartoon, decalInfo);
       
   //if (diffuseAlbedo.a == 0.f)
   //{
   //    return float4(diffuseAlbedo.xyz, 1.0f);
   //}
    
    float3 toEye = gEyePosW - position;

    float distToEye = length(toEye);

    toEye /= distToEye;

    // 현재 픽셀의 월드좌표에 광원의 VIEW,RPOJ행렬을 곱해 광원시점으로 이동시킴
    float4 worldPosByLight = WorldToLight(position);
    
    // 현재 픽셀의 광원시점에서의 Z값을 가져옴
    float vertexDepth = worldPosByLight.z / worldPosByLight.w;
    // 광원시점에서 오브젝트의 깊이를 담아놓은 텍스쳐에서 현재 픽셀의 Z값을 가져옴
    float lightDepth = gShadowDepthMapTexture.Sample(samAnisotropic, ToTexcoord(worldPosByLight)).r;
    
    float shadowFactor;
    
    // 본인 앞에 다른 물체가 있어 LIGHTDepth가 더 작다면 그림자가 생김
    if (vertexDepth > lightDepth + 0.0000125f)
    {
        shadowFactor = 0.5f;
    }
    else
    {
        shadowFactor = 1.0f;
    }
    
    float4 texColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    //diffuseAlbedo.a = 1.0f;
    //float4 texColor = float4(diffuseAlbedo.xyz, 1.0f);
    
    if (diffuseAlbedo.a <= 0.f)
    {
        cartoon = 0.0f;
        texColor = float4(diffuseAlbedo.xyz, 1.0f);
    }
    else
    {
        texColor = float4(diffuseAlbedo.xyz * shadowFactor, 1.0f);
    }
    
    Material nowMat;
    nowMat.Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    nowMat.Ambient = nowMat.Diffuse;
    nowMat.Specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    //nowMat.Ambient = float4(material.x, material.x, material.x, 1.0f);
    //nowMat.Diffuse = float4(material.y, material.y, material.y, 1.0f);
    //nowMat.Specular = float4(material.z, material.z, material.z, material.w);
    
    float3 toLightDir = -gDirLights[0].Direction;


   
	// Rim Lighting
    float rimIntensity = 0.2f;
    float rimThreshold = 0.2f;
    float rim = saturate(dot(normal, toLightDir)); // 시선 방향을 기준으로 Rim을 계산합니다
    rim = smoothstep(1.0f - rimThreshold, 1.0f, rim); // 임계값에 따라 Rim을 부드럽게 보정합니다
    float3 rimColor = float3(1.0f, 0.0f, 0.0f); // Rim 빛의 색상을 지정합니다 (필요에 따라 조절 가능)
    float4 rimLighting = rim * rimIntensity * float4(rimColor, 1.0f);

	// Toon Shading
	// NdotL
    //float NdotL = dot((texColor,1.0f), toLightDir);
    float NdotL = dot(normal, toLightDir);

    float4 litColor;
    float lightIntensity;
    float4 toonColor;
    
    // global ambient
    float4 ambient = float4(0.1f, 0.1f, 0.1f, 1.0f);
    
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    if (cartoon == 1.0f)
    {
       	// 빛의 강도
        lightIntensity = smoothstep(0, 1.f, NdotL);
        
            //float lightIntensity = NdotL > 0 ? 1 : 0;
	// 빛의 강도에 따라 색상 출력의 레이어를 정해준다 
        toonColor = ToonShade(lightIntensity);
    
        litColor = texColor * toonColor;
    }
    else
    { 
        for (int i = 0; i < gDirLightCount; ++i)
        {
            float4 A, D, S;
        
            ComputeDirectionalLight(nowMat, gDirLights[i], normal, toEye,
				A, D, S);
        
            ambient += A;
            diffuse += D;
            spec += S;
        }
    

        for (int j = 0; j < gPointLightCount; ++j)
        {
            float4 A, D, S;
        
            ComputePointLight(nowMat, gPointLights[j], position, normal, toEye,
				A, D, S, gAttenuation);
        
            ambient += A;
            diffuse += D;
            spec += S;
        }

        litColor = texColor * (ambient + diffuse) + spec;
    }
    

    
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
    float outlineThreshold = 0.99f; // 외곽선 감지 임계값
    float4 outlineColor = float4(0.0f, 0.0f, 0.0f, 1.0f); // 외곽선 색상
    float outlineFactor = saturate(1.0f - dot(normal, toLightDir)); // 노멀과 dirLight 간의 각도에 따라 외곽선 감도 결정
    float4 outline = outlineFactor > outlineThreshold ? outlineColor : float4(0.0f, 0.0f, 0.0f, 0.0f);

	
	// 원래 물체의 색상과 외곽선을 더함
    float4 finalColor = litColor; //+ outline + specular + rimLighting;
    
    float3 toneMappedColor = ToneMapReinhard(finalColor.xyz);
    
    if (decalInfo.x == 1)
    {
        //return float4(1.0f, 0.0f, 0.0f, 1.0f);
        float4 toDecal = mul(float4(position, 1.0f), gDecalWorldInv[decalInfo.y]);
        
        float minPos = -1.0f;
        float maxPos = 1.0f;
        
        if (minPos <= toDecal.x && toDecal.x <= maxPos)
        {
            if (minPos <= toDecal.y && toDecal.y <= maxPos)
            {
                if (minPos <= toDecal.z && toDecal.z <= maxPos)
                {
                    toneMappedColor.r += 0.5f;
                }
        
            }
        }
    }
    
    return float4(toneMappedColor, 1.0f);
    
    //return finalColor;
   
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