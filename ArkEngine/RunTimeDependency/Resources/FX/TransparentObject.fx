//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

Texture2D gDiffuseMap;
float gTransParency;
float gTime;

SamplerState samAnisotropic
{
    Filter = MIN_MAG_MIP_LINEAR;

    MaxAnisotropy = 4;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

struct PSOut
{
    float4 Diffuse : SV_Target;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    
    vout.Tex = vin.Tex;
    
    return vout;
}

PSOut PSBasic(VertexOut pin)
{
    PSOut output;
   
    // 텍스처 좌표를 사용하여 색상 샘플링
    output.Diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    
    float alpha = saturate(gTime);
    
    output.Diffuse.a = alpha * gTransParency;
    
    return output;
}

PSOut PSHorizontal(VertexOut pin)
{
    PSOut output;
   
    // 텍스처 좌표를 사용하여 색상 샘플링
    output.Diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    
    float alpha = (0.0f - pin.Tex.x) + gTime;
    
    alpha = saturate(alpha);
    
    alpha *= gTransParency;
    
    output.Diffuse.a = alpha;
    
    return output;
}

PSOut PSVertical(VertexOut pin)
{
    PSOut output;
   
    // 텍스처 좌표를 사용하여 색상 샘플링
    output.Diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    
    float alpha = (pin.Tex.y - 1.0f) + gTime;
    
    alpha = saturate(alpha);
    
    alpha *= gTransParency;
    
    output.Diffuse.a = alpha;
    
    return output;
}

PSOut PSCenter(VertexOut pin)
{
    PSOut output;
   
    // 텍스처 좌표를 사용하여 색상 샘플링
    output.Diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    
    float distanceFromCenter = length(pin.Tex - float2(0.5f, 0.5f));
    
    float maxDistance = length(float2(0.0f, 0.0f) - float2(0.5f, 0.5f));
    
    float alpha = (-1 * (distanceFromCenter + 0.5f)) + gTime;
    
    alpha = saturate(alpha);
    
    alpha *= gTransParency;
    
    output.Diffuse.a = alpha;
    
    return output;
}

PSOut PSCenterWithLine(VertexOut pin)
{
    PSOut output;
   
    // 텍스처 좌표를 사용하여 색상 샘플링
    output.Diffuse = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    
    float distanceFromCenter = length(pin.Tex - float2(0.5f, 0.5f));
    
    if (distanceFromCenter >= 0.499f)
    {
        output.Diffuse.a = 1.0f;
    }
    
    else
    {
        float maxDistance = length(float2(0.0f, 0.0f) - float2(0.5f, 0.5f));
    
        float alpha = (-1 * (distanceFromCenter + 0.5f)) + gTime;
    
        alpha = saturate(alpha);
    
        alpha *= gTransParency;
            
        output.Diffuse.a = alpha;
    }
    
    return output;
}

technique11 Tech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSBasic()));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSHorizontal()));
    }

    pass P2
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader(CompileShader(ps_5_0, PSVertical()));
    }

    pass P3
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSCenter()));
    }

    pass P4
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSCenterWithLine()));
    }
}
