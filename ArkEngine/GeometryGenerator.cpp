#include <DirectXMath.h>
#include <vector>
#include "ResourceManager.h"
#include "CommonStruct.h"
#include "ArkBuffer.h"
#include "GeometryGenerator.h"

void ArkEngine::ArkDX11::GeometryGenerator::CreateQuad()
{
	std::vector<Postex> vertexList;
	std::vector<unsigned int> indexList;

	unsigned int totalVertexCount = 4;
	unsigned int totalIndexCount = 6;

	vertexList.resize(totalVertexCount);

	vertexList[0].pos = {-1.0f, -1.0f, 0.0f};
	vertexList[0].tex = {0.0f, 1.0f};

	vertexList[1].pos = { -1.0f, 1.0f, 0.0f };
	vertexList[1].tex = { 0.0f, 0.0f };

	vertexList[2].pos = { 1.0f, 1.0f, 0.0f };
	vertexList[2].tex = { 1.0f, 0.0f };

	vertexList[3].pos = { 1.0f, -1.0f, 0.0f };
	vertexList[3].tex = { 1.0f, 1.0f };

	indexList.resize(totalIndexCount);

	indexList[0] = 0;
	indexList[1] = 1;
	indexList[2] = 2;
	indexList[3] = 0;
	indexList[4] = 2;
	indexList[5] = 3;

	ArkBuffer* newbuffer = new ArkBuffer("Quad", totalVertexCount, vertexList, totalIndexCount, indexList);
}

void ArkEngine::ArkDX11::GeometryGenerator::CreateBox(const char* geometryName, float width, float height, float depth)
{
	std::vector<Vertex> vertexList;
	std::vector<unsigned int> indexList;

	unsigned int totalVertexCount = 24;
	unsigned int totalIndexCount = 36;

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	float halfDepth = depth * 0.5f;

	vertexList.resize(totalVertexCount);

	// front face
	vertexList[0].pos = { -halfWidth, -halfHeight, -halfDepth };
	vertexList[0].norm = { 0.0f, 0.0f, -1.0f };
	vertexList[0].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[0].tex = { 0.0f, 1.0f };

	vertexList[1].pos = { -halfWidth, +halfHeight, -halfDepth };
	vertexList[1].norm = { 0.0f, 0.0f, -1.0f };
	vertexList[1].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[1].tex = { 0.0f, 0.0f };

	vertexList[2].pos = { +halfWidth, +halfHeight, -halfDepth };
	vertexList[2].norm = { 0.0f, 0.0f, -1.0f };
	vertexList[2].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[2].tex = { 1.0f, 0.0f };

	vertexList[3].pos = { +halfWidth, -halfHeight, -halfDepth };
	vertexList[3].norm = { 0.0f, 0.0f, -1.0f };
	vertexList[3].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[3].tex = { 1.0f, 1.0f };

	// back face
	vertexList[4].pos = { -halfWidth, -halfHeight, +halfDepth };
	vertexList[4].norm = { 0.0f, 0.0f, +1.0f };
	vertexList[4].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[4].tex = { 1.0f, 1.0f };

	vertexList[5].pos = { +halfWidth, -halfHeight, +halfDepth };
	vertexList[5].norm = { 0.0f, 0.0f, +1.0f };
	vertexList[5].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[5].tex = { 0.0f, 1.0f };

	vertexList[6].pos = { +halfWidth, +halfHeight, +halfDepth };
	vertexList[6].norm = { 0.0f, 0.0f, +1.0f };
	vertexList[6].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[6].tex = { 0.0f, 0.0f };

	vertexList[7].pos = { -halfWidth, +halfHeight, +halfDepth };
	vertexList[7].norm = { 0.0f, 0.0f, +1.0f };
	vertexList[7].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[7].tex = { 1.0f, 0.0f };

	// top face
	vertexList[8].pos = { -halfWidth, +halfHeight, -halfDepth };
	vertexList[8].norm = { 0.0f, 1.0f, 0.0f };
	vertexList[8].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[8].tex = { 0.0f, 1.0f };

	vertexList[9].pos = { -halfWidth, +halfHeight, +halfDepth };
	vertexList[9].norm = { 0.0f, 1.0f, 0.0f };
	vertexList[9].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[9].tex = { 0.0f, 0.0f };

	vertexList[10].pos = { +halfWidth, +halfHeight, +halfDepth };
	vertexList[10].norm = { 0.0f, 1.0f, 0.0f };
	vertexList[10].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[10].tex = { 1.0f, 0.0f };

	vertexList[11].pos = { +halfWidth, +halfHeight, -halfDepth };
	vertexList[11].norm = { 0.0f, 1.0f, 0.0f };
	vertexList[11].tangent = { 1.0f, 0.0f, 0.0f };
	vertexList[11].tex = { 1.0f, 1.0f };

	// bottom face
	vertexList[12].pos = { -halfWidth, -halfHeight, -halfDepth };
	vertexList[12].norm = { 0.0f, -1.0f, 0.0f };
	vertexList[12].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[12].tex = { 1.0f, 1.0f };

	vertexList[13].pos = { +halfWidth, -halfHeight, -halfDepth };
	vertexList[13].norm = { 0.0f, -1.0f, 0.0f };
	vertexList[13].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[13].tex = { 0.0f, 1.0f };

	vertexList[14].pos = {	+halfWidth, -halfHeight, +halfDepth };
	vertexList[14].norm = { 0.0f, -1.0f, 0.0f };
	vertexList[14].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[14].tex = { 0.0f, 0.0f };

	vertexList[15].pos = { -halfWidth, -halfHeight, +halfDepth };
	vertexList[15].norm = { 0.0f, -1.0f, 0.0f };
	vertexList[15].tangent = { -1.0f, 0.0f, 0.0f };
	vertexList[15].tex = { 1.0f, 0.0f };
	
	// left face
	vertexList[16].pos = { -halfWidth, -halfHeight, +halfDepth };
	vertexList[16].norm = { -1.0f, 0.0f, 0.0f };
	vertexList[16].tangent = { 0.0f, 0.0f, -1.0f };
	vertexList[16].tex = { 0.0f, 1.0f };

	vertexList[17].pos = { -halfWidth, +halfHeight, +halfDepth };
	vertexList[17].norm = { -1.0f, 0.0f, 0.0f };
	vertexList[17].tangent = { 0.0f, 0.0f, -1.0f };
	vertexList[17].tex = { 0.0f, 0.0f };

	vertexList[18].pos = { -halfWidth, +halfHeight, -halfDepth };
	vertexList[18].norm = { -1.0f, 0.0f, 0.0f };
	vertexList[18].tangent = { 0.0f, 0.0f, -1.0f };
	vertexList[18].tex = { 1.0f, 0.0f };

	vertexList[19].pos = { -halfWidth, -halfHeight, -halfDepth };
	vertexList[19].norm = { -1.0f, 0.0f, 0.0f };
	vertexList[19].tangent = { 0.0f, 0.0f, -1.0f };
	vertexList[19].tex = { 1.0f, 1.0f };

	// right face
	vertexList[20].pos = { +halfWidth, -halfHeight, -halfDepth };
	vertexList[20].norm = { 1.0f, 0.0f, 0.0f };
	vertexList[20].tangent = { 0.0f, 0.0f, 1.0f };
	vertexList[20].tex = { 0.0f, 1.0f };

	vertexList[21].pos = { +halfWidth, +halfHeight, -halfDepth };
	vertexList[21].norm = { 1.0f, 0.0f, 0.0f };
	vertexList[21].tangent = { 0.0f, 0.0f, 1.0f };
	vertexList[21].tex = { 0.0f, 0.0f };

	vertexList[22].pos = { +halfWidth, +halfHeight, +halfDepth };
	vertexList[22].norm = { 1.0f, 0.0f, 0.0f };
	vertexList[22].tangent = { 0.0f, 0.0f, 1.0f };
	vertexList[22].tex = { 1.0f, 0.0f };

	vertexList[23].pos = { +halfWidth, -halfHeight, +halfDepth };
	vertexList[23].norm = { 1.0f, 0.0f, 0.0f };
	vertexList[23].tangent = { 0.0f, 0.0f, 1.0f };
	vertexList[23].tex = { 1.0f, 1.0f };

	indexList.resize(totalIndexCount);

	indexList[0] = 0; indexList[1] = 1; indexList[2] = 2;
	indexList[3] = 0; indexList[4] = 2; indexList[5] = 3;

	// Fill in the back face index data
	indexList[6] = 4; indexList[7] = 5;  indexList[8] = 6;
	indexList[9] = 4; indexList[10] = 6; indexList[11] = 7;

	// Fill in the top face index data
	indexList[12] = 8; indexList[13] = 9;  indexList[14] = 10;
	indexList[15] = 8; indexList[16] = 10; indexList[17] = 11;

	// Fill in the bottom face index data
	indexList[18] = 12; indexList[19] = 13; indexList[20] = 14;
	indexList[21] = 12; indexList[22] = 14; indexList[23] = 15;

	// Fill in the left face index data
	indexList[24] = 16; indexList[25] = 17; indexList[26] = 18;
	indexList[27] = 16; indexList[28] = 18; indexList[29] = 19;

	// Fill in the right face index data
	indexList[30] = 20; indexList[31] = 21; indexList[32] = 22;
	indexList[33] = 20; indexList[34] = 22; indexList[35] = 23;

	ArkBuffer* newbuffer = new ArkBuffer(geometryName, totalVertexCount, vertexList, totalIndexCount, indexList);
}

void ArkEngine::ArkDX11::GeometryGenerator::CreateSphere(const char* geometryName, float radius, unsigned int sliceCount, unsigned int stackCount)
{
	std::vector<Vertex> vertexList;
	std::vector<unsigned int> indexList;

	Vertex topVertex;
	topVertex.pos = { 0.0f, radius, 0.0f };
	topVertex.norm = { 0.0f, 1.0f, 0.0f };
	topVertex.tangent = { 1.0f, 0.0f, 0.0f };
	topVertex.tex = { 0.0f, 0.0f };

	vertexList.emplace_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (unsigned int i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (unsigned int j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi);
			v.pos.z = radius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = +radius * sinf(phi) * cosf(theta);

			DirectX::XMVECTOR tangentV = DirectX::XMLoadFloat3(&v.tangent);
			DirectX::XMStoreFloat3(&v.tangent, DirectX::XMVector3Normalize(tangentV));

			DirectX::XMVECTOR postionV = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(&v.norm, DirectX::XMVector3Normalize(postionV));

			v.tex.x = theta / DirectX::XM_2PI;
			v.tex.y = phi / DirectX::XM_PI;

			vertexList.emplace_back(v);
		}
	}
	
	Vertex bottomVertex;
	bottomVertex.pos = { 0.0f, -radius, 0.0f };
	bottomVertex.norm = { 0.0f, -1.0f, 0.0f };
	bottomVertex.tangent = { 1.0f, 0.0f, 0.0f };
	bottomVertex.tex = { 0.0f, 1.0f };

	vertexList.emplace_back(bottomVertex);


	for (unsigned int i = 1; i <= sliceCount; ++i)
	{
		indexList.push_back(0);
		indexList.push_back(i + 1);
		indexList.push_back(i);
	}

	unsigned int baseIndex = 1;
	unsigned int ringVertexCount = sliceCount + 1;

	for (unsigned int i = 0; i < stackCount - 2; ++i)
	{
		for (unsigned int j = 0; j < sliceCount; ++j)
		{
			indexList.push_back(baseIndex + i * ringVertexCount + j);
			indexList.push_back(baseIndex + i * ringVertexCount + j + 1);
			indexList.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indexList.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indexList.push_back(baseIndex + i * ringVertexCount + j + 1);
			indexList.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	unsigned int southPoleIndex = (unsigned int)vertexList.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (unsigned int i = 0; i < sliceCount; ++i)
	{
		indexList.push_back(southPoleIndex);
		indexList.push_back(baseIndex + i);
		indexList.push_back(baseIndex + i + 1);
	}

	unsigned int totalVertexCount = (unsigned int) vertexList.size();
	unsigned int totalIndexCount = (unsigned int) indexList.size();

	ArkBuffer* newbuffer = new ArkBuffer(geometryName, totalVertexCount, vertexList, totalIndexCount, indexList);
}

void ArkEngine::ArkDX11::GeometryGenerator::CreateDebugBox(const char* geometryName, const DirectX::XMFLOAT3& centerPos, float width, float height, float depth, const DirectX::XMFLOAT4& color)
{
	std::vector<PosColor> vertexList;
	std::vector<unsigned int> indexList;

	unsigned int totalVertexCount = 8;
	unsigned int totalIndexCount = 36;

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	float halfDepth = depth * 0.5f;

	vertexList.resize(totalVertexCount);

	for (int i = 0; i < 8; ++i)
	{
		vertexList[i].color = color;
	}

	// Define 8 unique vertices
	vertexList[0].pos = { centerPos.x - halfWidth, centerPos.y, centerPos.z - halfDepth };
	vertexList[1].pos = { centerPos.x - halfWidth, centerPos.y + height, centerPos.z - halfDepth };
	vertexList[2].pos = { centerPos.x + halfWidth, centerPos.y + height, centerPos.z - halfDepth };
	vertexList[4].pos = { centerPos.x - halfWidth, centerPos.y, centerPos.z + halfDepth };
	vertexList[3].pos = { centerPos.x + halfWidth, centerPos.y, centerPos.z - halfDepth };
	vertexList[5].pos = { centerPos.x - halfWidth, centerPos.y + height, centerPos.z + halfDepth };
	vertexList[6].pos = { centerPos.x + halfWidth, centerPos.y + height, centerPos.z + halfDepth };
	vertexList[7].pos = { centerPos.x + halfWidth, centerPos.y, centerPos.z + halfDepth };

	indexList.resize(totalIndexCount);

	// Front face
	indexList[0] = 0; indexList[1] = 1; indexList[2] = 2;
	indexList[3] = 0; indexList[4] = 2; indexList[5] = 3;

	// Back face
	indexList[6] = 4; indexList[7] = 6; indexList[8] = 5;
	indexList[9] = 4; indexList[10] = 7; indexList[11] = 6;

	// Top face
	indexList[12] = 1; indexList[13] = 5; indexList[14] = 6;
	indexList[15] = 1; indexList[16] = 6; indexList[17] = 2;

	// Bottom face
	indexList[18] = 4; indexList[19] = 0; indexList[20] = 3;
	indexList[21] = 4; indexList[22] = 3; indexList[23] = 7;

	// Left face
	indexList[24] = 4; indexList[25] = 5; indexList[26] = 1;
	indexList[27] = 4; indexList[28] = 1; indexList[29] = 0;

	// Right face
	indexList[30] = 3; indexList[31] = 2; indexList[32] = 6;
	indexList[33] = 3; indexList[34] = 6; indexList[35] = 7;

	ArkBuffer* newbuffer = new ArkBuffer(geometryName, totalVertexCount, vertexList, totalIndexCount, indexList);

	std::vector<DirectX::XMFLOAT3> vertexPosList;

	for (int i = 0; i < vertexList.size(); i++)
	{
		vertexPosList.emplace_back(vertexList[i].pos);
	}
}

void ArkEngine::ArkDX11::GeometryGenerator::CreateDebugPhysXBox(const char* geometryName, const DirectX::XMFLOAT3& centerPos, float width, float height, float depth, const DirectX::XMFLOAT4& color)
{
	std::vector<PosColor> vertexList;
	std::vector<unsigned int> indexList;

	unsigned int totalVertexCount = 8;
	unsigned int totalIndexCount = 36;

	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	float halfDepth = depth * 0.5f;

	vertexList.resize(totalVertexCount);

	for (int i = 0; i < 8; ++i)
	{
		vertexList[i].color = color;
	}

	// Define 8 unique vertices
	vertexList[0].pos = { centerPos.x - halfWidth, centerPos.y - halfHeight, centerPos.z - halfDepth };
	vertexList[1].pos = { centerPos.x - halfWidth, centerPos.y + halfHeight, centerPos.z - halfDepth };
	vertexList[2].pos = { centerPos.x + halfWidth, centerPos.y + halfHeight, centerPos.z - halfDepth };
	vertexList[4].pos = { centerPos.x - halfWidth, centerPos.y - halfHeight, centerPos.z + halfDepth };
	vertexList[3].pos = { centerPos.x + halfWidth, centerPos.y - halfHeight, centerPos.z - halfDepth };
	vertexList[5].pos = { centerPos.x - halfWidth, centerPos.y + halfHeight, centerPos.z + halfDepth };
	vertexList[6].pos = { centerPos.x + halfWidth, centerPos.y + halfHeight, centerPos.z + halfDepth };
	vertexList[7].pos = { centerPos.x + halfWidth, centerPos.y - halfHeight, centerPos.z + halfDepth };

	indexList.resize(totalIndexCount);

	// Front face
	indexList[0] = 0; indexList[1] = 1; indexList[2] = 2;
	indexList[3] = 0; indexList[4] = 2; indexList[5] = 3;

	// Back face
	indexList[6] = 4; indexList[7] = 6; indexList[8] = 5;
	indexList[9] = 4; indexList[10] = 7; indexList[11] = 6;

	// Top face
	indexList[12] = 1; indexList[13] = 5; indexList[14] = 6;
	indexList[15] = 1; indexList[16] = 6; indexList[17] = 2;

	// Bottom face
	indexList[18] = 4; indexList[19] = 0; indexList[20] = 3;
	indexList[21] = 4; indexList[22] = 3; indexList[23] = 7;

	// Left face
	indexList[24] = 4; indexList[25] = 5; indexList[26] = 1;
	indexList[27] = 4; indexList[28] = 1; indexList[29] = 0;

	// Right face
	indexList[30] = 3; indexList[31] = 2; indexList[32] = 6;
	indexList[33] = 3; indexList[34] = 6; indexList[35] = 7;

	ArkBuffer* newbuffer = new ArkBuffer(geometryName, totalVertexCount, vertexList, totalIndexCount, indexList);

	std::vector<DirectX::XMFLOAT3> vertexPosList;

	for (int i = 0; i < vertexList.size(); i++)
	{
		vertexPosList.emplace_back(vertexList[i].pos);
	}
}

void ArkEngine::ArkDX11::GeometryGenerator::CreateDebugSphere(const char* geometryName, float minPos, float range, const DirectX::XMFLOAT4& color)
{
	float radius = range / 2;
	std::vector<PosColor> vertexList;
	std::vector<unsigned int> indexList;

	int stackCount = 10;
	int sliceCount = 20;

	PosColor topVertex;
	topVertex.pos = { 0.0f, minPos + range, 0.0f };
	topVertex.color = color;

	vertexList.emplace_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f * DirectX::XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (unsigned int i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring.
		for (unsigned int j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			PosColor v;

			// spherical to cartesian
			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi) + (minPos + radius);
			v.pos.z = radius * sinf(phi) * sinf(theta);

			v.color = color;

			vertexList.emplace_back(v);
		}
	}

	PosColor bottomVertex;
	bottomVertex.pos = { 0.0f, minPos, 0.0f };
	bottomVertex.color = color;

	vertexList.emplace_back(bottomVertex);

	for (unsigned int i = 1; i <= sliceCount; ++i)
	{
		indexList.push_back(0);
		indexList.push_back(i + 1);
		indexList.push_back(i);
	}

	unsigned int baseIndex = 1;
	unsigned int ringVertexCount = sliceCount + 1;

	for (unsigned int i = 0; i < stackCount - 2; ++i)
	{
		for (unsigned int j = 0; j < sliceCount; ++j)
		{
			indexList.push_back(baseIndex + i * ringVertexCount + j);
			indexList.push_back(baseIndex + i * ringVertexCount + j + 1);
			indexList.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indexList.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indexList.push_back(baseIndex + i * ringVertexCount + j + 1);
			indexList.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	unsigned int southPoleIndex = (unsigned int)vertexList.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (unsigned int i = 0; i < sliceCount; ++i)
	{
		indexList.push_back(southPoleIndex);
		indexList.push_back(baseIndex + i);
		indexList.push_back(baseIndex + i + 1);
	}

	unsigned int totalVertexCount = (unsigned int)vertexList.size();
	unsigned int totalIndexCount = (unsigned int)indexList.size();


	ArkBuffer* newbuffer = new ArkBuffer(geometryName, totalVertexCount, vertexList, totalIndexCount, indexList);
}