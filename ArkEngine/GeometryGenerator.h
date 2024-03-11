#pragma once

namespace DirectX
{
	struct XMFLOAT3;
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class GeometryGenerator
		{
		public:
			void CreateQuad();
			void CreateBox(const char* geometryName, float width, float height, float depth);
			void CreateSphere(const char* geometryName, float radius, unsigned int sliceCount, unsigned int stackCount);

			void CreateDebugBox(const char* geometryName, DirectX::XMFLOAT3 centerPos, float width, float height, float depth, DirectX::XMFLOAT4 color);
			void CreateDebugSphere(const char* geometryName, float minPos, float range, DirectX::XMFLOAT4 color);
		};
	}
}

