#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

namespace Rocket::Core
{
	/// <summary>
	/// MeshRenderer와 ModelRenderer가 본인이 들고 있는 메쉬, 머터리얼 정보를 이용해서 객체를 그릴때 사용한다.
	/// Render() 함수는 디퍼드를 상정하여 구현해야 한다..?
	/// </summary>
	class IRenderable
	{
	public:
		virtual void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj) = 0;
	};
}
