#pragma once

#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"
#include "../FloaterMath/include/Vector4f.h"
#include "../FloaterMath/include/Matrix4f.h"
#include <DirectXMath.h>

namespace flt
{
	DirectX::XMMATRIX ConvertXMMatrix(const Matrix4f& matrix);
}
