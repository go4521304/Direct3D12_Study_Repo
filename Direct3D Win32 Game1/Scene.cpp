#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

bool Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool Scene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void Scene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
	rootSigDesc.Init(0, NULL, 0, NULL, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> pSigBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob;

	DX::ThrowIfFailed(D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSigBlob, &pErrorBlob));
	DX::ThrowIfFailed(pd3dDevice->CreateRootSignature(0, pSigBlob->GetBufferPointer(), pSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature)));
}

void Scene::CreateGraphicsPipelineState(ID3D12Device* pd3dDevice)
{
	//���� ���̴��� �ȼ� ���̴��� �����Ѵ�.
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pPixelShader;

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	D3DCompileFromFile(L"Shaders.hlsl", NULL, NULL, "VSMain", "vs_5_1", nCompileFlags, 0, &pVertexShader, NULL);
	D3DCompileFromFile(L"Shaders.hlsl", NULL, NULL, "PSMain", "ps_5_1", nCompileFlags, 0, &pPixelShader, NULL);

	//�����Ͷ����� ���¸� �����Ѵ�. 
	D3D12_RASTERIZER_DESC rasterizerDesc;
	::ZeroMemory(&rasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//���� ���¸� �����Ѵ�. 
	D3D12_BLEND_DESC blendDesc;
	::ZeroMemory(&blendDesc, sizeof(D3D12_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//�׷��� ���������� ���¸� �����Ѵ�. 
	D3D12_GRAPHICS_PIPELINE_STATE_DESC posDesc;
	::ZeroMemory(&posDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	posDesc.pRootSignature = m_pd3dGraphicsRootSignature;
	posDesc.VS = CD3DX12_SHADER_BYTECODE(pVertexShader.Get());
	posDesc.PS = CD3DX12_SHADER_BYTECODE(pPixelShader.Get());
	posDesc.RasterizerState = rasterizerDesc;
	posDesc.BlendState = blendDesc;
	posDesc.DepthStencilState.DepthEnable = FALSE;
	posDesc.DepthStencilState.StencilEnable = FALSE;
	posDesc.InputLayout.pInputElementDescs = NULL;
	posDesc.InputLayout.NumElements = 0;
	posDesc.SampleMask = UINT_MAX;
	posDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	posDesc.NumRenderTargets = 1;
	posDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	posDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	posDesc.SampleDesc.Count = 1;
	posDesc.SampleDesc.Quality = 0;
	pd3dDevice->CreateGraphicsPipelineState(&posDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);
	
	//if (pVertexShader) pVertexShader->Release();
	//if (pPixelShader) pPixelShader->Release();
}

void Scene::BuildObjects(ID3D12Device* pd3dDevice)
{
}

void Scene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
}