#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "StaticMesh.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class CubeMesh : public StaticMesh
	{
	public:
		CubeMesh();
		~CubeMesh();

	public:
		virtual void Initialize(ID3D11Device* device) override;

	private:
		void BuildGeometryBuffers(ID3D11Device* device);
	};
}
