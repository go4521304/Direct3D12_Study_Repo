#include "pch.h"
#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
	m_pPso.clear();
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

	DX::ThrowIfFailed(pd3dDevice->CreateGraphicsPipelineState(&posDesc, IID_PPV_ARGS(m_pPso.back().GetAddressOf())));

	if (posDesc.InputLayout.pInputElementDescs)
		delete[] posDesc.InputLayout.pInputElementDescs;
}

void Shader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* commandList)
{ 
}

void Shader::ReleaseShaderVariables()
{
}

void Shader::UpdateShaderVariables(ID3D12GraphicsCommandList* commandList, XMFLOAT4X4* p4x4World)
{
	XMFLOAT4X4 m4x4World;;
	XMStoreFloat4x4(&m4x4World, XMMatrixTranspose(XMLoadFloat4x4(p4x4World)));
	commandList->SetGraphicsRoot32BitConstants(0, 16, &m4x4World, 0);
}

void Shader::OnPrepareRender(ID3D12GraphicsCommandList* commandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다. 
	commandList->SetPipelineState(m_pPso[0].Get());
}

void Shader::Render(ID3D12GraphicsCommandList* commandList, Camera* pCamera)
{
	OnPrepareRender(commandList);
}

/////////////////////////////////// PlayerShader ///////////////////////////////////

PlayerShader::PlayerShader()
{
}

PlayerShader::~PlayerShader()
{
}

D3D12_INPUT_LAYOUT_DESC PlayerShader::CreateInputLayout()
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

D3D12_SHADER_BYTECODE PlayerShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1",ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE PlayerShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1", ppd3dShaderBlob));
}

void PlayerShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	//그래픽스 파이프라인 상태 객체 배열을 생성한다.
	m_pPso.emplace_back();

	Shader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

/////////////////////////////////// PlayerShader ///////////////////////////////////

ObjectShader::ObjectShader()
{
}

ObjectShader::~ObjectShader()
{
}

D3D12_INPUT_LAYOUT_DESC ObjectShader::CreateInputLayout()
{
	UINT num = 2;
	D3D12_INPUT_ELEMENT_DESC* elementDesc;
	elementDesc = new D3D12_INPUT_ELEMENT_DESC[num];

	elementDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	elementDesc[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC layoutDesc;
	layoutDesc.NumElements = num;
	layoutDesc.pInputElementDescs = elementDesc;

	return layoutDesc;
}

D3D12_SHADER_BYTECODE ObjectShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSDiffused", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE ObjectShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSDiffused", "ps_5_1", ppd3dShaderBlob));
}

void ObjectShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dRootSignature)
{
	//그래픽스 파이프라인 상태 객체 배열을 생성한다.
	m_pPso.emplace_back();

	Shader::CreateShader(pd3dDevice, pd3dRootSignature);
}

void ObjectShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* commandList)
{
	//가로x세로x높이가 12x12x12인 정육면체 메쉬를 생성한다. 
	CubeMeshDiffused *pCubeMesh = new CubeMeshDiffused(pd3dDevice, commandList, 12.0f, 12.0f, 12.0f);
	
	/*x-축, y-축, z-축 양의 방향의 객체 개수이다. 각 값을 1씩 늘리거나 줄이면서 실행할 때 프레임 레이트가 어떻게 변하는 가를 살펴보기 바란다.*/
	int xObjects = 10, yObjects = 10, zObjects = 10, i = 0;

	//x-축, y-축, z-축으로 21개씩 총 21 x 21 x 21 = 9261개의 정육면체를 생성하고 배치한다.
	float fxPitch = 12.0f * 2.5f;
	float fyPitch = 12.0f * 2.5f;
	float fzPitch = 12.0f * 2.5f;
	RotatingObject* object = NULL;
	for (int x = -xObjects; x <= xObjects; x++)
	{
		for (int y = -yObjects; y <= yObjects; y++)
		{
			for (int z = -zObjects; z <= zObjects; z++)
			{
				object = new RotatingObject();
				object->SetMesh(pCubeMesh);
				//각 정육면체 객체의 위치를 설정한다. 
				object->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				object->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
				object->SetRotationSpeed(10.0f * (i % 10) + 3.0f);
				m_pObjects.push_back(object);
			}
		}
	}
	CreateShaderVariables(pd3dDevice, commandList);
}

void ObjectShader::ReleaseObjects()
{
	m_pObjects.clear();
}

void ObjectShader::AnimateObjects(float timeElapsed)
{
	for (const auto& obj : m_pObjects)
	{
		obj->Animate(timeElapsed);
	}
}

void ObjectShader::ReleaseUploadBuffers()
{
	for (const auto& obj : m_pObjects)
	{
		obj->ReleaseUploadBuffers();
	}
}

void ObjectShader::Render(ID3D12GraphicsCommandList* commandList, Camera* pCamera)
{
	Shader::Render(commandList, pCamera);

	for (const auto& obj : m_pObjects)
	{
		obj->Render(commandList, pCamera);
	}
}
