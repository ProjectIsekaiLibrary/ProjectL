//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

// 빛을 받는 객채의 포지션
Texture2D FinalTexture : register(t0);
Texture2D GrayScaleTexture1;
Texture2D GrayScaleTexture2;
Texture2D GrayScaleTexture3;
Texture2D GrayScaleTexture4;


SamplerState samAnisotropic
{
    Filter = MIN_MAG_MIP_LINEAR;
    MaxAnisotropy = 16;

    AddressU = WRAP;
    AddressV = WRAP;

    MipLODBias = 0.0;
    MinLOD = 0;
    MaxLOD = 15;
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

void GetGBufferAttributes(float2 texCoord, out float4 finalTexture, out float4 grayScale1, out float4 grayScale2, out float4 grayScale3, out float4 grayScale4)
{
    finalTexture = FinalTexture.Sample(samAnisotropic, texCoord);
    
    grayScale1 = GrayScaleTexture1.Sample(samAnisotropic, texCoord);

    grayScale2 = GrayScaleTexture2.Sample(samAnisotropic, texCoord);
    
    grayScale3 = GrayScaleTexture3.Sample(samAnisotropic, texCoord);
    
    grayScale4 = GrayScaleTexture4.Sample(samAnisotropic, texCoord);
}

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.Pos = float4(vin.Pos, 1.0f);
    vout.Tex = vin.Tex;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 finalTexture;
    
    float4 grayScale1;
    float4 grayScale2;
    float4 grayScale3;
    float4 grayScale4;
    
    GetGBufferAttributes(pin.Tex, finalTexture, grayScale1, grayScale2, grayScale3, grayScale4);
    
    float4 combinedGrayScale = (grayScale1 + grayScale2 + grayScale3 + grayScale4) * 0.4f;

    // Bloom intensity
    float bloomIntensity = 1.0f;

    //float4 result = finalTexture + grayScale1;
    // 최종 결과에 블러 및 그레이스케일 효과 추가
    float4 result = finalTexture + combinedGrayScale * bloomIntensity;
    
    return result;
}

technique11 Final
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}