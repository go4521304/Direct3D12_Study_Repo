#include "pch.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
	if (m_pPso)
	{
		m_pPso.Reset();
	}
}

//입력 조립기에게 정점 버퍼의 구조를 알려주기 위한 구조체를 반환한다. 
D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	//정점은 위치 벡터(POSITION)와 색상(COLOR)을 가진다. 
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
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

D3D12_SHADER_BYTECODE Shader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CompileShaderFromFile(L"Shaders.hlsl", "VSMain", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE Shader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CompileShaderFromFile(L"Shaders.hlsl", "PSMain", "ps_5_1", ppd3dShaderBlob));
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
	//그래픽스 파이프라인 상태 객체 배열을 생성한다. 
	m_nPipelineStates = 1;

	ComPtr<ID3D12PipelineState> m_ppd3dPipelineStates;
	m_pPso.Attach(m_ppd3dPipelineStates.Get());

	ComPtr<ID3DBlob> vertexShader, pixelShader;

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

	DX::ThrowIfFailed(pd3dDevice->CreateGraphicsPipelineState(&posDesc, IID_PPV_ARGS(m_pPso.GetAddressOf())));

	if (posDesc.InputLayout.pInputElementDescs)
		delete[] posDesc.InputLayout.pInputElementDescs;
}

void Shader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext)
{
	TriangleMesh* pTriangleMesh = new TriangleMesh(pd3dDevice, pd3dCommandList);

	m_pObjects.emplace_back(GameObject());
	m_pObjects.back()->SetMesh(pTriangleMesh);
}

void Shader::ReleaseObjects()
{
	m_pObjects.clear();
}

void Shader::AnimateObjects(float fTimeElapsed)
{
	for (auto i : m_pObjects)
	{
		i->Animate(fTimeElapsed);
	}
}

void Shader::ReleaseUploadBuffers()
{
	for (auto i : m_pObjects)
	{
		i->ReleaseUploadBuffers();
	}
}

void Shader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다. 
	pd3dCommandList->SetPipelineState(m_pPso.Get());
}

void Shader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	OnPrepareRender(pd3dCommandList);
	
	for (auto i : m_pObjects)
	{
		i->Render(pd3dCommandList);
	}
}