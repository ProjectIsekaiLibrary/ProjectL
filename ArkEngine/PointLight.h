/// <summary>
/// 
/// PointLight
/// 
/// À±Á¾È­
/// </summary>

#pragma once
#include "BasicLight.h"

namespace ArkEngine
{
	namespace ArkDX11
	{
		class DebugObject;
	}
}


namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT4;
}

namespace ArkEngine
{
	namespace ArkDX11
	{
		class PointLight : public ArkEngine::ArkDX11::BasicLight
		{
		public:
			PointLight(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, DirectX::XMFLOAT3 position, float radius);
			~PointLight();

		public:
			void SetPosition(DirectX::XMFLOAT3 position);
			void SetRange(float radius);
			void DeleteDebugObject();

		private:
			DirectX::XMFLOAT3 _position;
			float _radius;

			DirectX::XMFLOAT3 _att;
			float _pad;

		private:
			ArkEngine::ArkDX11::DebugObject* _debugObject;
		};
	}
}