#include "pch.h"
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

void Scene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//그래픽 루트 시그너쳐를 생성한다. 
	m_pRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	TriangleMesh* pMesh = new TriangleMesh(pd3dDevice, pd3dCommandList);

	RotatingObject* pObject = new RotatingObject();
	pObject->SetMesh(pMesh);

	CDiffusedShader* pShader = new CDiffusedShader();
	pShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	pShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	////씬을 그리기 위한 셰이더 객체를 생성한다. 
	//m_pShader.emplace_back(new Shader());
	//m_pShader.back()->CreateShader(pd3dDevice, m_pRootSignature.Get());
	//m_pShader.back()->BuildObjects(pd3dDevice, pd3dCommandList, NULL);
}

void Scene::ReleaseObjects()
{
	m_pRootSignature.Reset();

	for (auto i : m_pShader)
	{
		i->ReleaseShaderVariables();
		i->ReleaseObjects();
		i->Release();
	}
	m_pShader.clear();
}

void Scene::ReleaseUploadBuffers()
{
	for (auto i : m_pShader)
	{
		i->ReleaseUploadBuffers();
	}
}

// 왜 굳이 반환값으로 설정했을까?
ID3D12RootSignature* Scene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* rootSig = NULL;

	CD3DX12_ROOT_PARAMETER rootParm[2];
	rootParm[0].InitAsConstants(16, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	rootParm[1].InitAsConstants(32, 1, 0, D3D12_SHADER_VISIBILITY_VERTEX);

	D3D12_ROOT_SIGNATURE_FLAGS rootSigFlag =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
	rootSigDesc.Init(_countof(rootParm), rootParm, 0, NULL, rootSigFlag);

	Microsoft::WRL::ComPtr<ID3DBlob> pSigBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob;

	DX::ThrowIfFailed(D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSigBlob, &pErrorBlob));
	DX::ThrowIfFailed(pd3dDevice->CreateRootSignature(0, pSigBlob->GetBufferPointer(), pSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSig)));

	return rootSig;
}

ID3D12RootSignature* Scene::GetGraphicsRootSignature()
{
	return(m_pRootSignature.Get());
}

bool Scene::ProcessInput(UCHAR* pKeysBuffer)
{
	return false;
}

void Scene::AnimateObjects(float fTimeElapsed)
{
	for (auto i : m_pShader)
	{
		i->AnimateObjects(fTimeElapsed);
	}
}

void Scene::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pd3dCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());

	if (pCamera) pCamera->UpdateShaderVariables(pd3dCommandList);

	for (auto i : m_pShader)
	{
		i->Render(pd3dCommandList, pCamera);
	}
}