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

void GameObject::ReleaseUploadBuffers()
{
	//정점 버퍼를 위한 업로드 버퍼를 소멸시킨다.
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

	//게임 객체의 월드 변환 행렬을 셰이더의 상수 버퍼로 전달(복사)한다.
	if (m_pShader)
	{
		m_pShader->UpdateShaderVariables(pd3dCommandList, &m_xmf4x4World);
		m_pShader->Render(pd3dCommandList, pCamera);
	}

	// 게임 객체에 메쉬가 연결되어 있으면 메쉬를 렌더링한다.
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
