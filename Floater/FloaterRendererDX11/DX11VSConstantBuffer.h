#pragma once
#include <DirectXMath.h>

namespace flt
{
	struct VSConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldInvTrans;
		DirectX::XMMATRIX ViewProj;
		DirectX::XMMATRIX worldViewProj;
	};

	struct VSBoneConstantBuffer
	{
		DirectX::XMMATRIX boneTransform[96];
	};


	struct VSBackBuffer
	{
		// 0~1 사이 값. 
		float width;
		// 0~1 사이 값. 
		float height;
		// 이미지 중심의 좌표, 0~1 사이 값.  0일 시 왼쪽
		float x;
		// 이미지 중심의 좌표, 0~1 사이 값. 0일시 위
		float y;
	};
}

