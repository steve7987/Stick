#include "terraincell.h"

TerrainCell::TerrainCell(){
	vertexBuffer = 0;
	indexBuffer = 0;
	m_Texture = 0;
	m_TerrainData = 0;
}
	
TerrainCell::~TerrainCell(){

}


bool TerrainCell::Initialize(ID3D11Device * device, Vector * heightMap, Vector * normalMap, int startI, int startJ, int CellHeight, int CellWidth, 
							 int TotalWidth, WCHAR * textureFilename){
	if (!CreateInitialTerrain(heightMap, normalMap, startI, startJ, CellHeight, CellWidth, TotalWidth)){
		return false;
	}

	if (!InitializeBuffers(device, CellHeight, CellWidth)){
		return false;
	}

	m_Texture = g_textureManager->GetTexture(device, textureFilename);
	offset = Vector(0, 0, 0);

	return true;
}
	

void TerrainCell::Shutdown(){
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
	
	

bool TerrainCell::Render(ID3D11DeviceContext * deviceContext, Vector camlook){
	return RenderBuffers(deviceContext);
}

int TerrainCell::GetIndexCount(){
	return indexCount;
}

D3DXMATRIX TerrainCell::GetWorldMatrix(){
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, offset.x, offset.y, offset.z);
	return trans;
}

ID3D11ShaderResourceView* TerrainCell::GetTexture(int index){
	return m_Texture->GetTexture();
}


bool TerrainCell::HasAlpha(){
	return false;
}

float TerrainCell::getDepthSq(Vector campos, Vector look){
	return 0.0f;
}

void TerrainCell::SetOffset(Vector offset){
	this->offset = offset;
}

bool TerrainCell::RenderBuffers(ID3D11DeviceContext * deviceContext){
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

bool TerrainCell::CreateInitialTerrain(Vector * heightMap, Vector * normalMap, int startI, int startJ, int CellHeight, int CellWidth, int TotalWidth){
	vertexCount = (CellWidth) * (CellHeight) * 6;  //6 vertices per quad
	m_TerrainData = new ModelType[vertexCount];
	if (!m_TerrainData){
		textDump("Error creating terrain cell model");
		return false;
	}
	int index = 0;
	for (int i = startI; i < startI + CellHeight; i++){
		for (int j = startJ; j < startJ + CellWidth; j++){
			//calculate indices of the four corners
			int BL = (TotalWidth * i) + j;
			int BR = (TotalWidth * i) + j + 1;
			int UL = (TotalWidth * (i + 1)) + j;
			int UR = (TotalWidth * (i + 1)) + j + 1;

			//tri 1 UR
			m_TerrainData[index].position = heightMap[UR];
			m_TerrainData[index].tu = 0.0f;
			m_TerrainData[index].tv = 0.0f;
			m_TerrainData[index].normal = normalMap[UR];
			index++;
			
			//tri 1 UL
			m_TerrainData[index].position = heightMap[UL];
			m_TerrainData[index].tu = 1.0f;
			m_TerrainData[index].tv = 0.0f;
			m_TerrainData[index].normal = normalMap[UL];
			index++;

			//tri 1 BL
			m_TerrainData[index].position = heightMap[BL];
			m_TerrainData[index].tu = 1.0f;
			m_TerrainData[index].tv = 1.0f;
			m_TerrainData[index].normal = normalMap[BL];
			index++;

			//tri 2 UR
			m_TerrainData[index].position = heightMap[UR];
			m_TerrainData[index].tu = 0.0f;
			m_TerrainData[index].tv = 0.0f;
			m_TerrainData[index].normal = normalMap[UR];
			index++;

			//tri 2 BL
			m_TerrainData[index].position = heightMap[BL];
			m_TerrainData[index].tu = 1.0f;
			m_TerrainData[index].tv = 1.0f;
			m_TerrainData[index].normal = normalMap[BL];
			index++;

			//tri 2 BR
			m_TerrainData[index].position = heightMap[BR];
			m_TerrainData[index].tu = 0.0f;
			m_TerrainData[index].tv = 1.0f;
			m_TerrainData[index].normal = normalMap[BR];
			index++;

		}
	}
	return true;
}

bool TerrainCell::InitializeBuffers(ID3D11Device * device, int CellHeight, int CellWidth){
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	
	// Calculate the number of vertices in the terrain.
	vertexCount = (CellHeight) * (CellWidth) * 6;

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