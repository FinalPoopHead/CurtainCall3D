#include "GameWaitingSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

GameWaitingSceneView::GameWaitingSceneView()
	: scene()
{

}

void GameWaitingSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("GameWaiting");
	RocketEngine::LoadScene("GameWaiting");
	assert(scene);

	/// Object, Renderer 객체 생성
	
	// 게임 시작 버튼
	RocketEngine::GameObject* gameStartButtonObj = RocketEngine::CreateButton("gameStartButtonObj");
	RocketEngine::Button* gameStartButton = gameStartButtonObj->GetComponent<RocketEngine::Button>();
	
	// 방 설정 버튼
	RocketEngine::GameObject* roomSettingButtonObj = RocketEngine::CreateButton("roomSettingButtonObj");
	RocketEngine::Button* roomSettingButton = roomSettingButtonObj->GetComponent<RocketEngine::Button>();
	// 나가기 버튼
	RocketEngine::GameObject* closeButtonObj = RocketEngine::CreateButton("closeButtonObj");
	RocketEngine::Button* closeButton = closeButtonObj->GetComponent<RocketEngine::Button>();

	//////////////////////////////////////////////////////////////////////////

	/// Size 지정
	gameStartButtonObj->GetComponent<RocketEngine::Button>()->SetSize(350.0f, 150.0f);
	roomSettingButtonObj->GetComponent<RocketEngine::Button>()->SetSize(100.0f, 100.0f);
	closeButtonObj->GetComponent<RocketEngine::Button>()->SetSize(100.0f, 100.0f);

	/// 위치 지정
	gameStartButtonObj->transform.SetLocalPosition(910.0f / 2, 111.0f, 0.0f);
	roomSettingButtonObj->transform.SetLocalPosition(1677.0f / 2, 86.0f, 0.0f);
	closeButtonObj->transform.SetLocalPosition(1804.0f / 2, 86.0f, 0.0f);

	/// Text 설정

	/// Object 기본 상태 설정
	gameStartButtonObj->Enable();
	roomSettingButtonObj->Enable();
	closeButtonObj->Enable();

	/// Button 설정
	// 게임 시작 Button (InGame 으로 이동)
	gameStartButton->SetOnClickEvent(
		[gameStartButtonObj]()
		{

		}
	);

	// 방 설정 Button (설정 창으로 이동)
	roomSettingButton->SetOnClickEvent(
		[roomSettingButtonObj]()
		{

		}
	);

	// 나가기 Button (메인 메뉴로 이동)
	closeButton->SetOnClickEvent(
		[closeButtonObj]()
		{

		}
	);
}
