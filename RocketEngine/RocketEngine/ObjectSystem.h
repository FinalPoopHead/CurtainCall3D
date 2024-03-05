#pragma once
#include <vector>
#include <string>
#include "Singleton.h"

namespace Rocket
{
	class Scene;
	class GameObject;
	class Component;
}

namespace Rocket::Core
{
	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;
	private:
		ObjectSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();

		/// ������ ����Ŭ ����.
	public:
		void StartCurrentScene();

		// ���� ���� ���� Update�� �����Ѵ�.
		void UpdateCurrentScene();
		void LateUpdateCurrentScene();

		// ���õ� ���� ���� Update�� �����Ѵ�.
		void UpdateScene(Rocket::Scene* scene);

	public:
		void FlushEnable();
		void FlushDisable();

		void AddEnable(Rocket::GameObject* obj);
		void AddDisable(Rocket::GameObject* obj);

	public:
		Rocket::GameObject* CreateStaticObject(std::string objName);

	private:
		std::vector<Rocket::GameObject*> _staticObjList;

	public:
		void AddStaticComponent(Rocket::Component* component);

	private:
		std::vector<Rocket::Component*> _staticComponentList;

	private:
		std::vector<Rocket::GameObject*> _enableList;
		std::vector<Rocket::GameObject*> _disableList;
	};
}
