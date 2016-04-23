#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>

#include "classlist.h"
#include "graphics.h"
#include "texture.h"
#include "helpers.h"
#include "vector.h"
#include "texturemanager.h"
#include "renderable.h"
#include "terraincell.h"

extern Graphics * g_graphics;


class Terrain {
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
	
	Terrain();
	~Terrain();

	bool Initialize(ID3D11Device * device);
	void Shutdown();
	
	
	bool Render(float t);
	
	void Scroll(float amount);  //scrolls the terrain forward by the given amount

	float GetHeight(float x, float z);  //gives the y-height of the terrain at the given spot (adjusts for scroll)
	Vector GetNormal(float x, float z);  //gives the vector normal to the terrain at the given spot (adjusts for scroll)

private:
	
	bool InitializeHeightMap();  //creates a height map with all y values set to zero and normals to <0,1,0>
	void CalculateNormals();  //calculates the normals of the heightmap by weighting all the faces the vertice is part of
	
	void CreateHeightMap();  //modifies the y values of the height map to create a specific type of terrain
	void CreateSandDune(int begin, int end);  //creates a sand dune shape on the strip from begin to end

	void CreateTerrainCells(ID3D11Device * device);


	
	ModelType * m_TerrainData;
	

	Vector * m_HeightMap;
	Vector * m_NormalMap;

	Vector position;  //the terrain model has vertices from 0,0,0 to length, 0, width.  Position moves the model to different locations
	float currentScroll;  //gives the amount the terrain has been scrolled in the negative x direction

	TerrainCell ** m_TerrainCells;
};


#endif