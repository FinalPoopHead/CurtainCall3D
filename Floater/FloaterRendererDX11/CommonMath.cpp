#include "CommonMath.h"

DirectX::XMMATRIX flt::ConvertXMMatrix(const Matrix4f& matrix)
{
	DirectX::XMMATRIX result
	{
		matrix.e[0][0], matrix.e[0][1], matrix.e[0][2], matrix.e[0][3],
		matrix.e[1][0], matrix.e[1][1], matrix.e[1][2], matrix.e[1][3],
		matrix.e[2][0], matrix.e[2][1], matrix.e[2][2], matrix.e[2][3],
		matrix.e[3][0], matrix.e[3][1], matrix.e[3][2], matrix.e[3][3]
	};

	return result;
}
