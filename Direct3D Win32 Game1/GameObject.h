#pragma once
#include "Mesh.h"
using namespace std;


class Shader;
class Camera;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

private:
	int m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	XMFLOAT4X4 m_xmf4x4World;

	Shader *m_pShader = NULL;
	Mesh *m_pMesh = NULL;
	

public:
	// ��� ����
	ComPtr<ID3D12Resource> m_constantBuffer;
	ComPtr<ID3D12Resource> m_constantUploadBuffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC m_constantView;

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	void ReleaseUploadBuffers();

	virtual void SetShader(Shader* pShader);
	virtual void SetMesh(Mesh* pMesh);

	//���� ��ü�� ��ġ�� �����Ѵ�. 
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	virtual void Animate(float fTimeElapsed);

	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);		//���� ��ü�� ���� x-��, y-��, z-�� �������� �̵��Ѵ�.

	//���� ��ü�� ���� ��ȯ ��Ŀ��� ��ġ ���Ϳ� ����(x-��, y-��, z-��) ���͸� ��ȯ�Ѵ�. 
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera *pCamera);
};

class RotatingObject : public GameObject
{
public:
	RotatingObject();
	virtual ~RotatingObject();

private:
	XMFLOAT3 m_xmf3RotatingAxis;
	float m_fRotatingSpeed;

public:
	void SetRotateSpeed(float Speed) { m_fRotatingSpeed = Speed; }
	void SetRotateAxis(XMFLOAT3 Axis) { m_xmf3RotatingAxis = Axis; }

	virtual void Animate(float TimeElapsed);
};