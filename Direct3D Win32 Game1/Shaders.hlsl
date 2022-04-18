cbuffer cbGameObjInfo : register(b0)
{
	matrix gmtxWorld : packoffset(c0);
}

cbuffer cbCameraInfo : register(b1)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
}


struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


VS_OUTPUT VSDiffused(VS_INPUT input)
{
	VS_OUTPUT output;

	//정점의 위치 벡터는 투영좌표계로 표현되어 있으므로 변환하지 않고 그대로 출력한다. 
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);

	//입력되는 픽셀의 색상(래스터라이저 단계에서 보간하여 얻은 색상)을 그대로 출력한다. 
	output.color = input.color;

	return(output);
}

float4 PSDiffused(VS_OUTPUT input) : SV_TARGET
{
	return (input.color);
}