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
	XMFLOAT4X4 worldMatrix;

	Shader *m_pShader = NULL;
	Mesh *m_pMesh = NULL;
	

public:
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* commandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* commandList);
	virtual void ReleaseShaderVariables();

	void ReleaseUploadBuffers();

	virtual void SetShader(Shader* pShader);
	virtual void SetMesh(Mesh* pMesh);

	//���� ��ü�� ��ġ�� �����Ѵ�. 
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 position);

	virtual void Animate(float timeElapsed);

	void Rotate(XMFLOAT3* axis, float angle);
	void Rotate(float pitch = 10.0f, float yaw = 10.0f, float roll = 10.0f);		//���� ��ü�� ���� x-��, y-��, z-�� �������� �̵��Ѵ�.

	//���� ��ü�� ���� ��ȯ ��Ŀ��� ��ġ ���Ϳ� ����(x-��, y-��, z-��) ���͸� ��ȯ�Ѵ�. 
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	// ��ü�� ���� x, y, z �� �������� �̵�
	void MoveStrafe(float distance = 1.0f);
	void MoveUp(float distance = 1.0f);
	void MoveForward(float distance = 1.0f);

	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* commandList, Camera *pCamera);
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
	void SetRotationSpeed(float Speed) { m_fRotatingSpeed = Speed; }
	void SetRotationAxis(XMFLOAT3 Axis) { m_xmf3RotatingAxis = Axis; }

	virtual void Animate(float TimeElapsed);
};