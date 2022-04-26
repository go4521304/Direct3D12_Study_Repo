#include "pch.h"
#include "GameObject.h"
#include "Shader.h"
#include "Camera.h"

GameObject::GameObject()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}

GameObject::~GameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pShader)
	{
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
}

void GameObject::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//https://codingfarm.tistory.com/562
	DX::ThrowIfFailed(pd3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(XMFLOAT4X4)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_constantUploadBuffer.GetAddressOf())));

	m_constantView.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
	m_constantView.SizeInBytes = sizeof(XMFLOAT4X4);
}

void GameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));


	pd3dCommandList->SetGraphicsRootConstantBufferView(0, )
}

void GameObject::ReleaseShaderVariables()
{
}

void GameObject::ReleaseUploadBuffers()
{
	//���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��.
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();
}

void GameObject::SetShader(Shader* pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

void GameObject::SetMesh(Mesh* pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void GameObject::Animate(float fTimeElapsed)
{
}

void GameObject::Rotate(XMFLOAT3* pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void GameObject::OnPrepareRender()
{
}

void GameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera)
{
	OnPrepareRender();

	//���� ��ü�� ���� ��ȯ ����� ���̴��� ��� ���۷� ����(����)�Ѵ�.
	if (m_pShader)
	{
		m_pShader->UpdateShaderVariables(pd3dCommandList, &m_xmf4x4World);
		m_pShader->Render(pd3dCommandList, pCamera);
	}

	// ���� ��ü�� �޽��� ����Ǿ� ������ �޽��� �������Ѵ�.
	if (m_pMesh) m_pMesh->Render(pd3dCommandList);
}

RotatingObject::RotatingObject()
{
	m_xmf3RotatingAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotatingSpeed = 90.0f;
}

RotatingObject::~RotatingObject()
{
}

void RotatingObject::Animate(float TimeElapsed)
{
	GameObject::Rotate(&m_xmf3RotatingAxis, m_fRotatingSpeed * TimeElapsed);
}
