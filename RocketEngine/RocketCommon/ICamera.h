#pragma once
#include "IGraphicsObject.h"

namespace Rocket::Core
{
	class RocketTransform;
}

namespace Rocket::Core
{
	class ICamera : public IGraphicsObject
	{
	public:
		virtual void SetNearZ(float nearZ) = 0;
		virtual void SetFarZ(float farZ) = 0;
		virtual void SetAspect(float aspect) = 0;
		virtual void SetFOVY(float fov) = 0;
		virtual void SetAsMainCamera() = 0;			// 유일무이한 MainCamera로 세팅한다.
		virtual void AddToMainCamera() = 0;			// MainCamera 리스트에 추가한다. (현재는 2개짜리 배열)
		virtual void BindTransform(RocketTransform* transform) = 0;

		// Resize? viewport 사이즈 관리
		// 뷰포트 사이즈 변경 -> width, height
		// 뷰포트 사이즈 변경 -> 비율로 변경하게끔? -> widthRatio, heightRatio. ex) 0.5 0.5 면 전체 width의 절반 height의 절반?
		// 뷰포트 위치 변경 -> TopLeftX TopLeftY
		// 뷰포트 위치 변경 -> 비율에 맞게 위치 잡게끔? 흠..
		// 렌더타겟 사이즈 변경
	};
}
