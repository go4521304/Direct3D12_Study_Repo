#include "pch.h"
#include "GameObject.h"
#include "Shader.h"
#include "Camera.h"

GameObject::GameObject()
{
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
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

void GameObject::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* commandList)
{
}

void GameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* commandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix)));

	commandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}

void GameObject::ReleaseShaderVariables()
{
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

void GameObject::SetPosition(float x, float y, float z)
{
	worldMatrix._41 = x;
	worldMatrix._42 = y;
	worldMatrix._43 = z;
}

void GameObject::SetPosition(XMFLOAT3 position)
{
	SetPosition(position.x, position.y, position.z);
}

void GameObject::Animate(float timeElapsed)
{
}

void GameObject::Rotate(XMFLOAT3* axis, float angle)
{
	XMMATRIX rotateMatrix = XMMatrixRotationAxis(XMLoadFloat3(axis), XMConvertToRadians(angle));
	worldMatrix = Matrix4x4::Multiply(rotateMatrix, worldMatrix);
}

void GameObject::Rotate(float pitch, float yaw, float roll)
{
	XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll));
	worldMatrix = Matrix4x4::Multiply(rotateMatrix, worldMatrix);
}

XMFLOAT3 GameObject::GetPosition()
{
	return XMFLOAT3(worldMatrix._41, worldMatrix._42, worldMatrix._43);
}

XMFLOAT3 GameObject::GetLook()
{
	return Vector3::Normalize(XMFLOAT3(worldMatrix._31, worldMatrix._32, worldMatrix._33));
}

XMFLOAT3 GameObject::GetUp()
{
	return Vector3::Normalize(XMFLOAT3(worldMatrix._21, worldMatrix._22, worldMatrix._23));
}

XMFLOAT3 GameObject::GetRight()
{
	return Vector3::Normalize(XMFLOAT3(worldMatrix._11, worldMatrix._12, worldMatrix._13));
}

void GameObject::OnPrepareRender()
{
}

void GameObject::Render(ID3D12GraphicsCommandList* commandList, Camera* pCamera)
{
	OnPrepareRender();

	UpdateShaderVariables(commandList);

	//게임 객체의 월드 변환 행렬을 셰이더의 상수 버퍼로 전달(복사)한다.
	if (m_pShader) m_pShader->Render(commandList, pCamera);

	// 게임 객체에 메쉬가 연결되어 있으면 메쉬를 렌더링한다.
	if (m_pMesh) m_pMesh->Render(commandList);
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
