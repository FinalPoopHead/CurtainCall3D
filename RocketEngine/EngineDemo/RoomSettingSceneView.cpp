#include "RoomSettingSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

RoomSettingSceneView::RoomSettingSceneView()
	: scene()
{

}

void RoomSettingSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("RoomSetting");
	RocketEngine::LoadScene("RoomSetting");
	assert(scene);

	/// Object, Renderer ��ü ����
	
	/// ���� â
	// �� ���� Image
	RocketEngine::GameObject* roomSettingImageObj = RocketEngine::CreateImageBox("roomSettingImageObj");
	RocketEngine::SpriteRenderer* roomSettingSprite = roomSettingImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	// �ݱ� ��ư
	RocketEngine::GameObject* closeButtonObj = RocketEngine::CreateButton("closeButtonObj");
	RocketEngine::Button* closeButton = closeButtonObj->GetComponent<RocketEngine::Button>();
	
	// ���� Text
	RocketEngine::GameObject* roundTextObj = RocketEngine::CreateTextBox("roundTextObj");
	RocketEngine::TextBox* roundText = roundTextObj->GetComponent<RocketEngine::TextBox>();
	// ���� �� ���� Box
	RocketEngine::GameObject* roundValImageObj = RocketEngine::CreateImageBox("roundValImageObj");
	RocketEngine::SpriteRenderer* roundValISprite = roundValImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	// ���� �� Text
	RocketEngine::GameObject* roundValTextObj = RocketEngine::CreateTextBox("roundValTextObj");
	RocketEngine::TextBox* roundValText = roundValTextObj->GetComponent<RocketEngine::TextBox>();
	// ���� ���߱� ��ư
	RocketEngine::GameObject* roundDownButtonObj = RocketEngine::CreateButton("roundDownButtonObj");
	RocketEngine::Button* roundDownButton = roundDownButtonObj->GetComponent<RocketEngine::Button>();
	// ���� �ø��� ��ư
	RocketEngine::GameObject* roundUpButtonObj = RocketEngine::CreateButton("roundUpButtonObj");
	RocketEngine::Button* roundUpButton = roundUpButtonObj->GetComponent<RocketEngine::Button>();

	// �ð� Text
	RocketEngine::GameObject* timeTextObj = RocketEngine::CreateTextBox("timeTextObj");
	RocketEngine::TextBox* timeText = timeTextObj->GetComponent<RocketEngine::TextBox>();
	// �ð� ǥ�� Box
	RocketEngine::GameObject* timeValImageObj = RocketEngine::CreateImageBox("timeValImageObj");
	RocketEngine::SpriteRenderer* timeValSprite = timeValImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	// �ð� Text ǥ��
	RocketEngine::GameObject* timeValTextObj = RocketEngine::CreateTextBox("timeValTextObj");
	RocketEngine::TextBox* timeValText = timeValTextObj->GetComponent<RocketEngine::TextBox>();
	// �ð� Down ��ư
	RocketEngine::GameObject* timeDownButtonObj = RocketEngine::CreateButton("timeDownButtonObj");
	RocketEngine::Button* timeDownButton = timeDownButtonObj->GetComponent<RocketEngine::Button>();
	// �ð� Up ��ư
	RocketEngine::GameObject* timeUpButtonObj = RocketEngine::CreateButton("timeUpButtonObj");
	RocketEngine::Button* timeUpButton = timeUpButtonObj->GetComponent<RocketEngine::Button>();

	//////////////////////////////////////////////////////////////////////////

	/// Size ����
	closeButtonObj->GetComponent<RocketEngine::Button>()->SetSize(100.0f, 100.0f);
	roundDownButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);
	roundUpButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);
	timeDownButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);
	timeUpButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);

	/// ��ġ ����
	roomSettingImageObj->transform.SetLocalPosition(0.0f / 2, 575.0f, 0.0f);		// �ӽ� ���� ����
	closeButtonObj->transform.SetLocalPosition(1219.0f / 2, 270.0f, 0.0f);
	
	roundTextObj->transform.SetLocalPosition(959.0f / 2, 490.0f, 0.0f);	// ���� ����
	roundValImageObj->transform.SetLocalPosition(959.0f / 2, 590.0f, 0.0f);
	roundValTextObj->transform.SetLocalPosition(959.0f / 2, 590.0f, 0.0f);
	roundDownButtonObj->transform.SetLocalPosition(880.0f / 2, 590.0f, 0.0f);
	roundUpButtonObj->transform.SetLocalPosition(994.0f / 2, 590.0f, 0.0f);

	timeTextObj->transform.SetLocalPosition(959.0f / 2, 653, 0.0f);	// ���� ����
	timeValImageObj->transform.SetLocalPosition(870.0f / 2, 753.0f, 0.0f);
	timeValTextObj->transform.SetLocalPosition(870.0f / 2, 753.0f, 0.0f);	// ���� ����
	timeDownButtonObj->transform.SetLocalPosition(822.0f / 2, 753.0f, 0.0f);
	timeUpButtonObj->transform.SetLocalPosition(1097.0f / 2, 753.0f, 0.0f);

	/// Object �⺻ ���� ����
	roomSettingImageObj->Enable();
	closeButtonObj->Enable();
	roundTextObj->Enable();
	roundValImageObj->Enable();
	roundValTextObj->Enable();
	roundDownButtonObj->Enable();
	roundUpButtonObj->Enable();
	timeTextObj->Enable();
	timeValImageObj->Enable();
	timeValTextObj->Enable();
	timeDownButtonObj->Enable();
	timeUpButtonObj->Enable();

	/// Text ����
	roundText->SetText("����");
	roundValText->SetText("1");		// �ӽ�
	timeText->SetText("�ð�");
	timeValText->SetText("0:00");	// �ӽ�

	/// Button ��ȣ�ۿ�
	closeButton->SetOnClickEvent(
		[]() 
		{
		
		}
	);

	roundDownButton->SetOnClickEvent(
		[]()
		{

		}
	);

	roundUpButton->SetOnClickEvent(
		[]()
		{

		}
	);

	timeDownButton->SetOnClickEvent(
		[]()
		{

		}
	);

	timeUpButton->SetOnClickEvent(
		[]()
		{

		}
	);

}
