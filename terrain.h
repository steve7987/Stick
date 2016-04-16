#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>

#include "classlist.h"
#include "texture.h"
#include "helpers.h"
#include "vector.h"
#include "texturemanager.h"
#include "renderable.h"

extern TextureManager * g_textureManager;

class Terrain : public Renderable{
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
	struct HeightMapType{
		Vector position;
		Vector normal;
	};

public:
	Terrain();
	~Terrain();

	bool Initialize(ID3D11Device * device);
	void Shutdown();
	
	
	bool Render(ID3D11DeviceContext * deviceContext, Vector camlook);
	int GetIndexCount();
	D3DXMATRIX GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture(int index = 0);

	bool HasAlpha();
	float getDepthSq(Vector campos, Vector look);

	void Scroll(float amount);  //scrolls the terrain forward by the given amount

private:
	bool RenderBuffers(ID3D11DeviceContext * deviceContext);

	bool InitializeBuffers(ID3D11Device * device);
	bool InitializeHeightMap();  //creates a height map with all y values set to zero and normals to <0,1,0>
	void CalculateNormals();  //calculates the normals of the heightmap by weighting all the faces the vertice is part of
	bool CreateInitialTerrain();

	void CreateHeightMap();  //modifies the y values of the height map to create a specific type of terrain
	void CreateSandDune(int begin, int end);  //creates a sand dune shape on the strip from begin to end


	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;
	int indexCount, vertexCount;

	Texture * m_Texture;
	ModelType * m_TerrainData;
	

	HeightMapType * m_HeightMap;

	Vector position;  //the terrain model has vertices from 0,0,0 to length, 0, width.  Position moves the model to different locations
};


#endif