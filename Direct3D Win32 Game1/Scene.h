#pragma once
#include "Shader.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	~Scene();

	//������ ���콺�� Ű���� �޽����� ó���Ѵ�. 
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* commandList);
	void ReleaseObjects();

	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float timeElapsed);
	void Render(ID3D12GraphicsCommandList* commandList, Camera* pCamera);

	void ReleaseUploadBuffers();

	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�. 
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();

private:
	ComPtr<ID3D12RootSignature>     m_pRootSignature;
	ComPtr<ID3D12PipelineState>		m_pPipelineState;

protected:
	vector<unique_ptr<ObjectShader>> m_pShader;
};