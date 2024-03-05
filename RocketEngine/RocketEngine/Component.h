#pragma once
#include "DLLExporter.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket::Core
{
	class ObjectSystem;
	class GraphicsSystem;
}

/// <summary>
/// 컴포넌트의 베이스 클래스.
/// IComponent 인터페이스를 상속받고 빈 구현으로 둔다.
/// 다른 컴포넌트가 이 클래스를 상속받는다.
/// 이게 맞냐? -> 기능별로 인터페이스를 나눠?
/// 
/// 23.06.29 강석원 인재원.
/// </summary>
namespace Rocket
{
	class ROCKET_API Component
	{
		friend class GameObject;
		friend class Core::ObjectSystem;
		friend class Core::GraphicsSystem;
	
	public:
		Component()
			: gameObject(nullptr)
		{

		}

		virtual ~Component() = default;

	protected:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void FixedUpdate() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void Finalize() {}

	protected:
		virtual void OnDestroy() {}
		virtual void OnCollisionEnter() {}
		virtual void OnCollisionStay() {}
		virtual void OnCollisionExit() {}

	protected:
		virtual void UpdateRenderData() {}


	public:
		GameObject* gameObject;

	private:
		void SetRestart();

	private:
		bool _isStarted = false;
	};
}
