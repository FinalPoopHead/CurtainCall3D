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
/// 게임의 씬 클래스.
/// 
/// 23.06.30 강석원 인재원.
/// </summary>
namespace Rocket
{
	class ROCKET_API Scene
	{
		/// 생성자, 소멸자.
		/// 시리얼라이즈가 Scene의 데이터를 받아와야 함
		friend class SAD::Serialize;
	public:
		Scene(std::string sceneName, bool fromEditor = false);
		~Scene();


		/// Scene LifeCycle.
	public:
		// 등록되어있는 정보로 runningObject 갱신.
		void Initialize();

		void Start();

		// Scene에 있는 Object 업데이트.
		void Update();

		void LateUpdate();

		// Scene 이 Unload 되면 호출. RunningList 정리.
		void Finalize();

		/// Scene에 Object 생성.
	public:
		GameObject* CreateObject(std::string objName);
		GameObject* CreateStaticObject(std::string objName);
		bool DeleteObject(std::string gameObjectName);


		/// mainCamera 관련.
	public:
		Camera* GetMainCamera();
		void SetMainCamera(Camera* mainCamera);

	private:
		Camera* _mainCamera;


		/// SceneName 관련.
	public:
		std::string GetSceneName();
		void SetSceneName(const std::string& sceneName);

	private:
		std::string _sceneName;


	public:
		GameObject* FindObjectByName(std::string name);

		/// Scene에 등록되어있는 Object 관리.
	public:
		std::vector<GameObject*>& GetOriginalList();	// -> serialize
		std::vector<GameObject*>& GetRunningList();


	private:
		// 원본 리스트
		std::vector<GameObject*> _originalList;

		// 현재 리스트
		std::vector<GameObject*> _runningList;


		/// Scene의 UI 관련
	public:
		void CheckFocus();

	public:
		void AddUI(Rocket::Core::UIRenderer* ui);

	private:
		Rocket::Core::UIRenderer* _focusedObj;
		std::vector<Rocket::Core::UIRenderer*> _uiComponents;
	};
}
 