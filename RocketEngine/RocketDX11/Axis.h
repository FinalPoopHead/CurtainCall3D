#pragma once
#include <d3d11.h>
#include <dxgi.h>

#include "HelperObjectBase.h"

namespace Rocket::Core
{
	class Axis : public HelperObjectBase
	{
	public:
		virtual void Initialize(ID3D11Device* device) override;

	private:
		void BuildGeometryBuffers(ID3D11Device* device);
	};
}
