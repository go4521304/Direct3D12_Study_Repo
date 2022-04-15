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
	void ReleaseUploadBuffers();

	virtual void SetShader(Shader* pShader);
	virtual void SetMesh(Mesh* pMesh);

	virtual void Animate(float fTimeElapsed);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);


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