#pragma once
#include"DLLExporter.h"
#include <vector>
#include <string>
#include <functional>

// namespace Rocket
// {
// 	class GameObject;
// }
// 
// template class __declspec(dllexport) std::vector<Rocket::GameObject*>;
// __declspec(dllexport) std::string;

namespace SAD
{
	class Serialize;
}

namespace Rocket
{
	class GameObject;
	class Camera;
	class Serialize;
}

namespace Rocket::Core
{
	class UIRenderer;
}

/// <summary>
/// ������ �� Ŭ����.
/// 
/// 23.06.30 ������ �����.
/// </summary>
namespace Rocket
{
	class ROCKET_API Scene
	{
		/// ������, �Ҹ���.
		/// �ø������� Scene�� �����͸� �޾ƿ;� ��
		friend class SAD::Serialize;
	public:
		Scene(std::string sceneName, bool fromEditor = false);
		~Scene();


		/// Scene LifeCycle.
	public:
		// ��ϵǾ��ִ� ������ runningObject ����.
		void Initialize();

		void Start();

		// Scene�� �ִ� Object ������Ʈ.
		void Update();

		void LateUpdate();

		// Scene �� Unload �Ǹ� ȣ��. RunningList ����.
		void Finalize();

		/// Scene�� Object ����.
	public:
		GameObject* CreateObject(std::string objName);
		GameObject* CreateStaticObject(std::string objName);
		bool DeleteObject(std::string gameObjectName);


		/// mainCamera ����.
	public:
		Camera* GetMainCamera();
		void SetMainCamera(Camera* mainCamera);

	private:
		Camera* _mainCamera;


		/// SceneName ����.
	public:
		std::string GetSceneName();
		void SetSceneName(const std::string& sceneName);

	private:
		std::string _sceneName;


	public:
		GameObject* FindObjectByName(std::string name);

		/// Scene�� ��ϵǾ��ִ� Object ����.
	public:
		std::vector<GameObject*>& GetOriginalList();	// -> serialize
		std::vector<GameObject*>& GetRunningList();


	private:
		// ���� ����Ʈ
		std::vector<GameObject*> _originalList;

		// ���� ����Ʈ
		std::vector<GameObject*> _runningList;


		/// Scene�� UI ����
	public:
		void CheckFocus();

	public:
		void AddUI(Rocket::Core::UIRenderer* ui);

	private:
		Rocket::Core::UIRenderer* _focusedObj;
		std::vector<Rocket::Core::UIRenderer*> _uiComponents;
	};
}
 