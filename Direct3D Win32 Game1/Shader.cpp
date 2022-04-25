#include "pch.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
	//for (auto i : m_pPso)
	//{
	//	delete i;
	//}
	delete[] m_pPso;
}


D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	DX::ThrowIfFailed(D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, NULL));

	return CD3DX12_SHADER_BYTECODE(*ppd3dShaderBlob);
}

//입력 조립기에게 정점 버퍼의 구조를 알려주기 위한 구조체를 반환한다. 
D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC layoutDesc;
	layoutDesc.pInputElementDescs = NULL;
	layoutDesc.NumElements = 0;

	return layoutDesc;
}

D3D12_SHADER_BYTECODE Shader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	CD3DX12_SHADER_BYTECODE shaderByteCode{NULL, 0};
	return shaderByteCode;
}

D3D12_SHADER_BYTECODE Shader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	CD3DX12_SHADER_BYTECODE shaderByteCode{ NULL, 0 };
	return shaderByteCode;
}

D3D12_RASTERIZER_DESC Shader::CreateRasterizerState()
{
	return CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
}

D3D12_BLEND_DESC Shader::CreateBlendState()
{
	return CD3DX12_BLEND_DESC(D3D12_DEFAULT);
}

D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState()
{
	return CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
}

void Shader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dRootSignature)
{
	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC posDesc;
	::ZeroMemory(&posDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	posDesc.pRootSignature = pd3dRootSignature;
	posDesc.VS = CreateVertexShader(vertexShader.GetAddressOf());
	posDesc.PS = CreatePixelShader(pixelShader.GetAddressOf());
	posDesc.RasterizerState = CreateRasterizerState();
	posDesc.BlendState = CreateBlendState();
	posDesc.DepthStencilState = CreateDepthStencilState();
	posDesc.InputLayout = CreateInputLayout();
	posDesc.SampleMask = UINT_MAX;
	posDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	posDesc.NumRenderTargets = 1;
	posDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	posDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	posDesc.SampleDesc.Count = 1;

	DX::ThrowIfFailed(pd3dDevice->CreateGraphicsPipelineState(&posDesc, IID_PPV_ARGS(&m_pPso[0])));

	if (posDesc.InputLayout.pInputElementDescs)
		delete[] posDesc.InputLayout.pInputElementDescs;
}

void Shader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{ 
}

void Shader::ReleaseShaderVariables()
{
}

void Shader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* p4x4World)
{
	XMFLOAT4X4 m4x4World;;
	XMStoreFloat4x4(&m4x4World, XMMatrixTranspose(XMLoadFloat4x4(p4x4World)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &m4x4World, 0);
}

void Shader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다. 
	pd3dCommandList->SetPipelineState(m_pPso[0]);
}

void Shader::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	OnPrepareRender(pd3dCommandList);
}

/////////////////////////////////// DiffusedShader ///////////////////////////////////

DiffusedShader::DiffusedShader()
{
}

DiffusedShader::~DiffusedShader()
{
}

D3D12_INPUT_LAYOUT_DESC DiffusedShader::CreateInputLayout()
{
	UINT num = 2;
	D3D12_INPUT_ELEMENT_DESC *elementDesc;
	elementDesc = new D3D12_INPUT_ELEMENT_DESC[num];

	elementDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	elementDesc[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC layoutDesc;
	layoutDesc.NumElements = num;
	layoutDesc.pInputElementDescs = elementDesc;

	return layoutDesc;
}

D3D12_SHADER_BYTECODE DiffusedShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE DiffusedShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1", ppd3dShaderBlob));
}

void DiffusedShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	//그래픽스 파이프라인 상태 객체 배열을 생성한다.
	m_pPso = new ID3D12PipelineState * [1];

	Shader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}
