#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

namespace Rocket::Core
{
	class IRenderable
	{
	public:
		virtual void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) = 0;
	};
}
