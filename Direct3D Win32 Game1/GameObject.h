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
	// 상수 버퍼
	ComPtr<ID3D12Resource> m_constantBuffer;
	ComPtr<ID3D12Resource> m_constantUploadBuffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC m_constantView;

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();

	void ReleaseUploadBuffers();

	virtual void SetShader(Shader* pShader);
	virtual void SetMesh(Mesh* pMesh);

	//게임 객체의 위치를 설정한다. 
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	virtual void Animate(float fTimeElapsed);

	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);		//게임 객체를 로컬 x-축, y-축, z-축 방향으로 이동한다.

	//게임 객체의 월드 변환 행렬에서 위치 벡터와 방향(x-축, y-축, z-축) 벡터를 반환한다. 
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