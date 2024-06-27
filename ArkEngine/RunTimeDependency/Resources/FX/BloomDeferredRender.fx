//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

// ºûÀ» ¹Þ´Â °´Ã¤ÀÇ Æ÷Áö¼Ç
Texture2D FinalTexture : register(t0);

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

void GetGBufferAttributes(float2 texCoord, out float4 finalTexture, float scale)
{
    float2 scaledTexCoord = texCoord * float2(scale, scale);
    finalTexture = FinalTexture.Sample(samAnisotropic, scaledTexCoord);
}

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    vout.Pos = float4(vin.Pos, 1.0f);
    vout.Tex = vin.Tex;
    
    return vout;
}

float4 PS16(VertexOut pin) : SV_Target
{
    float4 finalTexture;
    
    GetGBufferAttributes(pin.Tex, finalTexture, 16.0f);
        
    float4 graycolor = float4(0.2627f, 0.6780f, 0.0593f, 0.0f);
    
    float4 bb = dot(finalTexture, graycolor);
    bb -= 0.7f;
    bb = saturate(bb);
    
    bb.a = 1.0f;
    
    return bb;
}

float4 PS8(VertexOut pin) : SV_Target
{
    float4 finalTexture;
    
    GetGBufferAttributes(pin.Tex, finalTexture, 8.0f);

    float4 graycolor = float4(0.2627f, 0.6780f, 0.0593f, 0.0f);
    
    float4 bb = dot(finalTexture, graycolor);
    bb -= 0.7f;
    bb = saturate(bb);
    
    bb.a = 1.0f;
    
    return bb;
}

float4 PS4(VertexOut pin) : SV_Target
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

float4 PS2(VertexOut pin) : SV_Target
{
    float4 finalTexture;
    
    GetGBufferAttributes(pin.Tex, finalTexture, 2.0f);
        
    float4 graycolor = float4(0.2627f, 0.6780f, 0.0593f, 0.0f);
    
    float4 bb = dot(finalTexture, graycolor);
    bb -= 0.7f;
    bb = saturate(bb);
    
    bb.a = 1.0f;
    
    return bb;
}

technique11 Final
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS16()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS8()));
    }

    pass P2
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS4()));
    }

    pass P3
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS2()));
    }
}