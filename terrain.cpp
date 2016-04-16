#include "terrain.h"

#define TERRAIN_WIDTH 257
#define TERRAIN_HEIGHT 1025
#define TERRAIN_TEXTURE L"./assets/sand.dds"

//should be divisor of Terrain width - 1
#define CELL_WIDTH 64
//should be divisor of Terrain height - 1
#define CELL_HEIGHT 64

Terrain::Terrain(){
	m_TerrainData = 0;
	m_HeightMap = 0;
	m_NormalMap = 0;

	m_TerrainCells = 0;
}
	
Terrain::~Terrain(){

}


bool Terrain::Initialize(ID3D11Device * device){
	position = Vector(-20, -12, -TERRAIN_WIDTH / 2.0f);
	
	if (!InitializeHeightMap()){
		return false;
	}
	
	CreateHeightMap();
	CalculateNormals();

	CreateTerrainCells(device);

	currentScroll = 0;

	return true;
}

void Terrain::Shutdown(){
	if (m_TerrainCells){
		for (int i = 0; i < (TERRAIN_WIDTH - 1) / CELL_WIDTH * (TERRAIN_HEIGHT - 1) / CELL_HEIGHT; i++){
			m_TerrainCells[i]->Shutdown();
			delete m_TerrainCells[i];
			m_TerrainCells[i] = 0;
		}
		delete [] m_TerrainCells;
		m_TerrainCells = 0;
	}

	if (m_HeightMap){
		delete m_HeightMap;
		m_HeightMap = 0;
	}
	if (m_NormalMap){
		delete m_NormalMap;
		m_NormalMap = 0;
	}

	if (m_TerrainData){
		delete m_TerrainData;
		m_TerrainData = 0;
	}
}
	
bool Terrain::Render(float t){
	for (int i = 0; i < (TERRAIN_WIDTH - 1) / CELL_WIDTH * (TERRAIN_HEIGHT - 1) / CELL_HEIGHT; i++){
		g_graphics->RenderObject(m_TerrainCells[i], SHADER_LIGHT);
	}
	return true;
}

bool Terrain::InitializeHeightMap(){
	m_HeightMap = new Vector[TERRAIN_WIDTH * TERRAIN_HEIGHT];
	for (int i = 0; i < TERRAIN_HEIGHT; i++){
		for (int j = 0; j < TERRAIN_WIDTH; j++){
			m_HeightMap[i * TERRAIN_WIDTH + j] = Vector(i, 0, j);
		}
	}
	return true;
}

void Terrain::CalculateNormals(){
	//calculate normals for each face
	Vector * faceNormals = new Vector[(TERRAIN_WIDTH - 1) * (TERRAIN_HEIGHT - 1)];
	for (int i = 0; i < TERRAIN_HEIGHT - 1; i++){
		for (int j = 0; j < TERRAIN_WIDTH - 1; j++){
			//get 3 indices for the face
			int BL = (TERRAIN_WIDTH * i) + j;
			int BR = (TERRAIN_WIDTH * i) + j + 1;
			int UL = (TERRAIN_WIDTH * (i + 1)) + j;
			//calculate the normal
			Vector v1 = m_HeightMap[BR] - m_HeightMap[BL];
			Vector v2 = m_HeightMap[UL] - m_HeightMap[BL];
			faceNormals[i * (TERRAIN_WIDTH - 1) + j] = v1.cross(v2).normalize();
		}
	}
	m_NormalMap = new Vector[TERRAIN_WIDTH * TERRAIN_HEIGHT];

	//for each vertex average the face normals
	for (int i = 0; i < TERRAIN_HEIGHT; i++){
		for (int j = 0; j < TERRAIN_WIDTH; j++){
			Vector normSum = Vector(0, 0, 0);
			//bottom left face
			if (i - 1 >= 0 && j - 1 >= 0){
				normSum = normSum + faceNormals[(i - 1) * (TERRAIN_WIDTH - 1) + j - 1];
			}
			//bottom right face
			if (i - 1 >= 0 && j < TERRAIN_WIDTH - 1){
				normSum = normSum + faceNormals[(i - 1) * (TERRAIN_WIDTH - 1) + j];
			}
			//top left face
			if (i < TERRAIN_HEIGHT - 1 && j - 1 >= 0){
				normSum = normSum + faceNormals[i * (TERRAIN_WIDTH - 1) + j - 1];
			}
			//top right face
			if (i < TERRAIN_HEIGHT - 1 && j < TERRAIN_WIDTH - 1){
				normSum = normSum + faceNormals[i * (TERRAIN_WIDTH - 1) + j];
			}
			//store the average of the face normals
			m_NormalMap[i * TERRAIN_WIDTH + j] = normSum.normalize();
		}
	}
	delete [] faceNormals;
	faceNormals = 0;
}

void Terrain::CreateHeightMap(){
	CreateSandDune(TERRAIN_WIDTH / 5, TERRAIN_WIDTH / 2);
	CreateSandDune(TERRAIN_WIDTH / 2, 4 * TERRAIN_WIDTH / 5);
}

void Terrain::CreateSandDune(int begin, int end){
	float maxHeight = 12.0f + randb(-3, 3);
	float offsetPercent = 0.15f + randb(0.01, 0.04);
	float Zm = 0.72f + randb(-0.1, 0.1);  //constant for where the top of the sand dune is
	for (int i = 0; i < TERRAIN_HEIGHT; i++){
		float zOffset = sin((float) i / TERRAIN_HEIGHT * 2 * PI) * offsetPercent;
		for (int j = 0; j < TERRAIN_WIDTH; j++){
			float heightPercent = 0;
			//calculate the percentage across the [begin, end] interval this vertex is
			float zpercent = (float) (j - begin) / (end - begin) + zOffset;
			if (zpercent >= 0 && zpercent < Zm){  //windward side
				heightPercent = 0.5f * (1 - cos(PI * zpercent / Zm));
			}
			else if (zpercent >= Zm && zpercent <= 1.0f) {  //slip side
				heightPercent = 1 - cos(PI / 2.0f * (zpercent - 1) / (Zm - 1));
			}
			m_HeightMap[i * TERRAIN_WIDTH + j].y += maxHeight * heightPercent;
		}
	}
}

void Terrain::Scroll(float amount){
	currentScroll += amount;
	Vector scrollOffset;
	for (int i = 0; i < (TERRAIN_HEIGHT - 1) / CELL_HEIGHT; i++){
		for (int j = 0; j < (TERRAIN_WIDTH - 1) / CELL_WIDTH; j++){
			int index = i * (TERRAIN_WIDTH - 1) / CELL_WIDTH + j;
			if ((i + 1) * CELL_HEIGHT < currentScroll){
				scrollOffset = Vector(currentScroll - TERRAIN_HEIGHT + 1, 0, 0);
			}
			else {
				scrollOffset = Vector(currentScroll, 0, 0);
			}
			m_TerrainCells[index]->SetOffset(position - scrollOffset);
		}
	}
	if (currentScroll > TERRAIN_HEIGHT){
		currentScroll -= TERRAIN_HEIGHT;
	}
}

void Terrain::CreateTerrainCells(ID3D11Device * device){
	m_TerrainCells = new TerrainCell *[(TERRAIN_WIDTH - 1) / CELL_WIDTH * (TERRAIN_HEIGHT - 1) / CELL_HEIGHT];
	for (int i = 0; i < (TERRAIN_HEIGHT - 1) / CELL_HEIGHT; i++){
		for (int j = 0; j < (TERRAIN_WIDTH - 1) / CELL_WIDTH; j++){
			int index = i * (TERRAIN_WIDTH - 1) / CELL_WIDTH + j;
			m_TerrainCells[index] = new TerrainCell();
			m_TerrainCells[index]->Initialize(device, m_HeightMap, m_NormalMap, i * CELL_HEIGHT, j * CELL_WIDTH, CELL_HEIGHT, CELL_WIDTH, TERRAIN_WIDTH, TERRAIN_TEXTURE);
			m_TerrainCells[index]->SetOffset(position);
		}
	}
}