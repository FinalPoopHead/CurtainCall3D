#pragma once
namespace Rocket::Core
{
	/// <summary>
	/// 리소스들을 한번에 묶는 리소스 인터페이스
	/// 이걸 상속받아서 여러 리소스들이 만들어진다.
	/// 우선은 2D, 3D로 나눌 예정
	/// 그리고 세이더나 그런것도..
	/// </summary>
	class IResource
	{
// 	public:
// 		virtual void Initialize(ID3D11Device* device) = 0;
	};
}
