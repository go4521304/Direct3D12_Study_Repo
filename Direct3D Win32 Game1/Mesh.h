#pragma once
class Vertex
{
protected:
	XMFLOAT3 m_xmf3Position;

public:
	Vertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	Vertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~Vertex() {}
};

class DiffusedVertex : public Vertex
{
protected:
	XMFLOAT4 m_xmf4Diffuse;

public:
	DiffusedVertex() 
	{
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); 
		m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	DiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse) 
	{
		m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse;
	}
	DiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse) 
	{
		m_xmf3Position = xmf3Position; m_xmf4Diffuse = xmf4Diffuse;
	}
	~DiffusedVertex() { }
};

class Mesh
{
public:
	Mesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~Mesh();

private:
	int m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	void ReleaseUploadBuffers();

protected:
	ComPtr<ID3D12Resource> m_pd3dVertexBuffer;
	ComPtr<ID3D12Resource> m_pd3dVertexUploadBuffer;

	D3D12_VERTEX_BUFFER_VIEW m_d3dVertexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT m_nSlot = 0;
	UINT m_nVertices = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

/*�ε��� ����(�ε����� �迭)�� �ε��� ���۸� ���� ���ε� ���ۿ� ���� �������̽� �������̴�. �ε��� ���۴� ���� ����(�迭)�� ���� �ε����� ������.*/
protected:
	ComPtr<ID3D12Resource> m_indexBuffer;
	ComPtr<ID3D12Resource> m_indexUploadBuffer;

	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	UINT m_nIndices = 0;	// �ε����� ����
	UINT m_nStartIndex = 0;	// �޽��� �׸��� ���� ����ϴ� ���� �ε���
	UINT m_nBaseVertex = 0;	// �ε��� ������ �ε����� ������ �ε���

public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
};

class TriangleMesh : public Mesh
{
public:
	TriangleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~TriangleMesh() { }
};

class CubeMeshDiffused : public Mesh
{
public:
	CubeMeshDiffused(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CubeMeshDiffused();
};