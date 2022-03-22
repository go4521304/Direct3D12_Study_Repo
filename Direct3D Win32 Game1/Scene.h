#pragma once

class Scene
{
public:
	Scene();
	~Scene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	void CreateGraphicsPipelineState(ID3D12Device* pd3dDevice);

	void BuildObjects(ID3D12Device* pd3dDevice);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed);

	void Render(ID3D12GraphicsCommandList* pd3dCommandList);

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature>     m_pRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>		m_pPipelineState;
};