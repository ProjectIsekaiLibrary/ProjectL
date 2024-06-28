//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

// 빛을 받는 객채의 포지션
Texture2D FinalTexture : register(t0);

SamplerState samAnisotropic
{
    Filter = MIN_MAG_MIP_LINEAR;
    MaxAnisotropy = 16;

    AddressU = ClAMP;
    AddressV = ClAMP;

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

void GetGBufferAttributes(float2 texCoord, out float4 finalTexture, float scale)
{
    float2 scaledTexCoord = texCoord * float2(scale, scale);
    finalTexture = FinalTexture.Sample(samAnisotropic, scaledTexCoord);
}

float gaussWeight(int x)
{
    float sigma = 5.0f;
    return exp(-float(x * x) / (2.0f * sigma * sigma)) / (sqrt(2.0f * 3.14159265f) * sigma);
}

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.Pos = float4(vin.Pos, 1.0f);
    vout.Tex = vin.Tex;
    
    return vout;
}

float4 PSGrayScale(VertexOut pin) : SV_Target
{
    float4 finalTexture;
    
    GetGBufferAttributes(pin.Tex, finalTexture, 4.0f);

    float4 graycolor = float4(0.2627f, 0.6780f, 0.0593f, 0.0f);
    
    float4 bb = dot(finalTexture, graycolor);
    bb -= 0.7f;
    bb = saturate(bb);
    
    bb.a = 1.0f;
    
    return bb;
}

float4 PSBlurVertical(VertexOut pin) : SV_Target
{
    float4 sum = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float weightSum = 0.0f;
    
    // 1 / 270.0f;
    float2 texelSize = float2(0.0f, 0.00370f);
    
    // 가우시안 블러링 수행
    for (int i = -4; i <= 4; ++i)
    {
        float weight = gaussWeight(i);
        
        // 텍스처 좌표에 4배 스케일링을 적용하여 샘플링
        sum += FinalTexture.Sample(samAnisotropic, (pin.Tex + float2(0.0f, i) * texelSize) * float2(4.0f, 4.0f)) * weight;
        weightSum += weight;
    }
    
    // 블러링된 결과 반환
    return sum / weightSum;
}

float4 PSBlurHorizontal(VertexOut pin) : SV_Target
{
    float4 sum = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float weightSum = 0.0f;
    
    // 1 / 480.0f;
    float2 texelSize = float2(0.00208f, 0.0f);
    
    // 가우시안 블러링 수행
    for (int i = -4; i <= 4; ++i)
    {
        float weight = gaussWeight(i);
        
        // 텍스처 좌표에 4배 스케일링을 적용하여 샘플링
        sum += FinalTexture.Sample(samAnisotropic, (pin.Tex + float2(i, 0.0f) * texelSize) * float2(4.0f, 4.0f)) * weight;
        weightSum += weight;
    }
    
    // 블러링된 결과 반환
    return sum / weightSum;
}

technique11 Final
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSGrayScale()));
    }
    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSBlurVertical()));
    }
    pass P2
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSBlurHorizontal()));
    }
}