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

	/// Object, Renderer ��ü ����
	
	// ���� ���� ��ư
	RocketEngine::GameObject* gameStartButtonObj = RocketEngine::CreateButton("gameStartButtonObj");
	RocketEngine::Button* gameStartButton = gameStartButtonObj->GetComponent<RocketEngine::Button>();
	
	// �� ���� ��ư
	RocketEngine::GameObject* roomSettingButtonObj = RocketEngine::CreateButton("roomSettingButtonObj");
	RocketEngine::Button* roomSettingButton = roomSettingButtonObj->GetComponent<RocketEngine::Button>();
	// ������ ��ư
	RocketEngine::GameObject* closeButtonObj = RocketEngine::CreateButton("closeButtonObj");
	RocketEngine::Button* closeButton = closeButtonObj->GetComponent<RocketEngine::Button>();

	//////////////////////////////////////////////////////////////////////////

	/// Size ����
	gameStartButtonObj->GetComponent<RocketEngine::Button>()->SetSize(350.0f, 150.0f);
	roomSettingButtonObj->GetComponent<RocketEngine::Button>()->SetSize(100.0f, 100.0f);
	closeButtonObj->GetComponent<RocketEngine::Button>()->SetSize(100.0f, 100.0f);

	/// ��ġ ����
	gameStartButtonObj->transform.SetLocalPosition(910.0f / 2, 111.0f, 0.0f);
	roomSettingButtonObj->transform.SetLocalPosition(1677.0f / 2, 86.0f, 0.0f);
	closeButtonObj->transform.SetLocalPosition(1804.0f / 2, 86.0f, 0.0f);

	/// Text ����

	/// Object �⺻ ���� ����
	gameStartButtonObj->Enable();
	roomSettingButtonObj->Enable();
	closeButtonObj->Enable();

	/// Button ����
	// ���� ���� Button (InGame ���� �̵�)
	gameStartButton->SetOnClickEvent(
		[gameStartButtonObj]()
		{

		}
	);

	// �� ���� Button (���� â���� �̵�)
	roomSettingButton->SetOnClickEvent(
		[roomSettingButtonObj]()
		{

		}
	);

	// ������ Button (���� �޴��� �̵�)
	closeButton->SetOnClickEvent(
		[closeButtonObj]()
		{

		}
	);
}
