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
	CreateGraphicsRootSignature(pd3dDevice);

	//씬을 그리기 위한 셰이더 객체를 생성한다. 
	m_pShader.emplace_back(new Shader());
	m_pShader.back()->CreateShader(pd3dDevice, m_pRootSignature.Get());
	m_pShader.back()->BuildObjects(pd3dDevice, pd3dCommandList, NULL);
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

void Scene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
	rootSigDesc.Init(0, NULL, 0, NULL, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> pSigBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> pErrorBlob;

	DX::ThrowIfFailed(D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSigBlob, &pErrorBlob));
	DX::ThrowIfFailed(pd3dDevice->CreateRootSignature(0, pSigBlob->GetBufferPointer(), pSigBlob->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.GetAddressOf())));
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

void Scene::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());

	for (auto i : m_pShader)
	{
		i->Render(pd3dCommandList);
	}
}