#ifndef _TERRAINCELL_H_
#define _TERRAINCELL_H_

#include <D3D11.h>
#include <D3DX10math.h>

#include "classlist.h"
#include "texture.h"
#include "helpers.h"
#include "vector.h"
#include "texturemanager.h"
#include "renderable.h"


extern TextureManager * g_textureManager;

class TerrainCell : public Renderable{
private:
	//vertex type, needs to match same type in shader class
	struct VertexType{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct ModelType{
		Vector position;
		float tu, tv;
		Vector normal;
	};

	

public:
	TerrainCell();
	~TerrainCell();

	//i, j represents the starting lower left vertex.  Ending vertex is i+ch, j+cw
	bool Initialize(ID3D11Device * device, Vector * heightMap, Vector * normalMap, int startI, int startJ, int CellHeight, int CellWidth, 
					int TotalWidth, WCHAR * textureFilename);
	
	void Shutdown();
	
	
	bool Render(ID3D11DeviceContext * deviceContext, Vector camlook);
	int GetIndexCount();
	D3DXMATRIX GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture(int index = 0);

	bool HasAlpha();
	float getDepthSq(Vector campos, Vector look);

	void SetOffset(Vector offset);


private:
	bool RenderBuffers(ID3D11DeviceContext * deviceContext);
	bool CreateInitialTerrain(Vector * heightMap, Vector * normalMap, int startI, int startJ, int CellHeight, int CellWidth, int TotalWidth);
	bool InitializeBuffers(ID3D11Device * device, int CellHeight, int CellWidth);

	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;
	int indexCount, vertexCount;

	Texture * m_Texture;
	ModelType * m_TerrainData;

	Vector offset;  //gives the offset from the default position in the world
};


#endif