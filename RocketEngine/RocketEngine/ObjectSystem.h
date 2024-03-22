#pragma once
#include <vector>
#include <string>
#include "Singleton.h"

namespace Rocket
{
	class Scene;
	class GameObject;
	class Component;
	class DynamicModelRenderer;
}

namespace Rocket::Core
{
	struct RawNode;
	class RocketTransform;
}

namespace Rocket::Core
{
	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;
	private:
		ObjectSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		void Initialize();
		void Finalize();

		/// 라이프 사이클 관리.
	public:
		void StartCurrentScene();

		// 현재 씬에 대해 Update를 수행한다.
		void UpdateCurrentScene();
		void LateUpdateCurrentScene();

		// 선택된 씬에 대해 Update를 수행한다.
		void UpdateScene(Rocket::Scene* scene);

	public:
		void FlushEnable();
		void FlushDisable();

		void AddEnable(Rocket::GameObject* obj);
		void AddDisable(Rocket::GameObject* obj);

	public:
		Rocket::GameObject* CreateObject(const std::string& objName);
		Rocket::GameObject* CreateStaticObject(const std::string& objName);
		Rocket::GameObject* CreateModelObject(const std::string& fileName);

		void AddStaticComponent(Rocket::Component* component);

	private:
		Rocket::GameObject* CreateStaticMeshObjectRecur(RawNode* node);		// 모델렌더러도 붙은 Hierarchy가 완성된 root GameObject를 반환한다.
		Rocket::GameObject* CreateDynamicModelObjectRecur(RawNode* node, Rocket::DynamicModelRenderer** outModelRenderer);		// 모델렌더러도 붙은 Hierarchy가 완성된 root GameObject를 반환한다.

	private:
		std::vector<Rocket::GameObject*> _staticObjList;
		std::vector<Rocket::Component*> _staticComponentList;

	private:
		std::vector<Rocket::GameObject*> _enableList;
		std::vector<Rocket::GameObject*> _disableList;
	};
}
