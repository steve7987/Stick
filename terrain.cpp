#include "terrain.h"

#define TERRAIN_WIDTH 256
#define TERRAIN_HEIGHT 256
#define TERRAIN_TEXTURE L"./assets/sand.dds"

Terrain::Terrain(){
	vertexBuffer = 0;
	indexBuffer = 0;
	m_Texture = 0;
	m_TerrainData = 0;
	m_HeightMap = 0;
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

	if (!CreateInitialTerrain()){
		return false;
	}
	
	if (!InitializeBuffers(device)){
		return false;
	}

	m_Texture = g_textureManager->GetTexture(device, TERRAIN_TEXTURE);

	return true;
}

void Terrain::Shutdown(){
	if (m_HeightMap){
		delete m_HeightMap;
		m_HeightMap = 0;
	}
	if (m_TerrainData){
		delete m_TerrainData;
		m_TerrainData = 0;
	}
	if (vertexBuffer){
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
	if (indexBuffer){
		indexBuffer->Release();
		indexBuffer = 0;
	}
}
	
bool Terrain::Render(ID3D11DeviceContext * deviceContext, Vector camlook){
	return RenderBuffers(deviceContext);
}
	
int Terrain::GetIndexCount(){
	return indexCount;
}

D3DXMATRIX Terrain::GetWorldMatrix(){
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, position.x, position.y, position.z);
	return trans;
}

ID3D11ShaderResourceView* Terrain::GetTexture(int index){
	return m_Texture->GetTexture();
}


bool Terrain::HasAlpha(){
	return false;
}

float Terrain::getDepthSq(Vector campos, Vector look){
	return 0.0f;
}


bool Terrain::RenderBuffers(ID3D11DeviceContext * deviceContext){
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}



bool Terrain::InitializeBuffers(ID3D11Device * device){
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	
	// Calculate the number of vertices in the terrain.
	vertexCount = (TERRAIN_HEIGHT - 1) * (TERRAIN_WIDTH - 1) * 6;

	// Set the index count to the same as the vertex count.
	indexCount = vertexCount;

	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if(!indices)
	{
		return false;
	}
	
	// Load the vertex array and index array with 3D terrain model data.
	for(i=0; i < vertexCount; i++)
	{
		vertices[i].position = m_TerrainData[i].position.d3dvector();
		vertices[i].texture = D3DXVECTOR2(m_TerrainData[i].tu, m_TerrainData[i].tv);
		vertices[i].normal = m_TerrainData[i].normal.d3dvector();
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

bool Terrain::CreateInitialTerrain(){
	vertexCount = (TERRAIN_WIDTH - 1) * (TERRAIN_HEIGHT - 1) * 6;  //6 vertices per quad
	m_TerrainData = new ModelType[vertexCount];
	if (!m_TerrainData){
		textDump("Error creating terrain model");
		return false;
	}
	int index = 0;
	for (int i = 0; i < TERRAIN_HEIGHT - 1; i++){
		for (int j = 0; j < TERRAIN_WIDTH - 1; j++){
			//calculate indices of the four corners
			int BL = (TERRAIN_WIDTH * i) + j;
			int BR = (TERRAIN_WIDTH * i) + j + 1;
			int UL = (TERRAIN_WIDTH * (i + 1)) + j;
			int UR = (TERRAIN_WIDTH * (i + 1)) + j + 1;

			//tri 1 UR
			m_TerrainData[index].position = m_HeightMap[UR].position;
			m_TerrainData[index].tu = 0.0f;
			m_TerrainData[index].tv = 0.0f;
			m_TerrainData[index].normal = m_HeightMap[UR].normal;
			index++;
			
			//tri 1 UL
			m_TerrainData[index].position = m_HeightMap[UL].position;
			m_TerrainData[index].tu = 1.0f;
			m_TerrainData[index].tv = 0.0f;
			m_TerrainData[index].normal = m_HeightMap[UL].normal;
			index++;

			//tri 1 BL
			m_TerrainData[index].position = m_HeightMap[BL].position;
			m_TerrainData[index].tu = 1.0f;
			m_TerrainData[index].tv = 1.0f;
			m_TerrainData[index].normal = m_HeightMap[BL].normal;
			index++;

			//tri 2 UR
			m_TerrainData[index].position = m_HeightMap[UR].position;
			m_TerrainData[index].tu = 0.0f;
			m_TerrainData[index].tv = 0.0f;
			m_TerrainData[index].normal = m_HeightMap[UR].normal;
			index++;

			//tri 2 BL
			m_TerrainData[index].position = m_HeightMap[BL].position;
			m_TerrainData[index].tu = 1.0f;
			m_TerrainData[index].tv = 1.0f;
			m_TerrainData[index].normal = m_HeightMap[BL].normal;
			index++;

			//tri 2 BR
			m_TerrainData[index].position = m_HeightMap[BR].position;
			m_TerrainData[index].tu = 0.0f;
			m_TerrainData[index].tv = 1.0f;
			m_TerrainData[index].normal = m_HeightMap[BR].normal;
			index++;

		}
	}
	return true;
}

bool Terrain::InitializeHeightMap(){
	m_HeightMap = new HeightMapType[TERRAIN_WIDTH * TERRAIN_HEIGHT];
	for (int i = 0; i < TERRAIN_HEIGHT; i++){
		for (int j = 0; j < TERRAIN_WIDTH; j++){
			m_HeightMap[i * TERRAIN_WIDTH + j].position = Vector(i, 0, j);
			m_HeightMap[i * TERRAIN_WIDTH + j].normal = Vector(0, 1, 0);
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
			Vector v1 = m_HeightMap[BR].position - m_HeightMap[BL].position;
			Vector v2 = m_HeightMap[UL].position - m_HeightMap[BL].position;
			faceNormals[i * (TERRAIN_WIDTH - 1) + j] = v1.cross(v2).normalize();
		}
	}
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
			m_HeightMap[i * TERRAIN_WIDTH + j].normal = normSum.normalize();
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
			m_HeightMap[i * TERRAIN_WIDTH + j].position.y += maxHeight * heightPercent;
		}
	}
}

void Terrain::Scroll(float amount){
	this->position.x -= amount;
}