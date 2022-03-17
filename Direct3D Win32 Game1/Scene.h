#pragma once
#include "pch.h"

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

	void PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList);

private:
	ID3D12RootSignature*							m_pd3dGraphicsRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>		m_pd3dPipelineState;
};