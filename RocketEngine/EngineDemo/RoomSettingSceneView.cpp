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

	/// Object, Renderer 객체 생성
	
	/// 설정 창
	// 방 설정 Image
	RocketEngine::GameObject* roomSettingImageObj = RocketEngine::CreateImageBox("roomSettingImageObj");
	RocketEngine::SpriteRenderer* roomSettingSprite = roomSettingImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	// 닫기 버튼
	RocketEngine::GameObject* closeButtonObj = RocketEngine::CreateButton("closeButtonObj");
	RocketEngine::Button* closeButton = closeButtonObj->GetComponent<RocketEngine::Button>();
	
	// 라운드 Text
	RocketEngine::GameObject* roundTextObj = RocketEngine::CreateTextBox("roundTextObj");
	RocketEngine::TextBox* roundText = roundTextObj->GetComponent<RocketEngine::TextBox>();
	// 라운드 값 담을 Box
	RocketEngine::GameObject* roundValImageObj = RocketEngine::CreateImageBox("roundValImageObj");
	RocketEngine::SpriteRenderer* roundValISprite = roundValImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	// 라운드 값 Text
	RocketEngine::GameObject* roundValTextObj = RocketEngine::CreateTextBox("roundValTextObj");
	RocketEngine::TextBox* roundValText = roundValTextObj->GetComponent<RocketEngine::TextBox>();
	// 라운드 낮추기 버튼
	RocketEngine::GameObject* roundDownButtonObj = RocketEngine::CreateButton("roundDownButtonObj");
	RocketEngine::Button* roundDownButton = roundDownButtonObj->GetComponent<RocketEngine::Button>();
	// 라운드 올리기 버튼
	RocketEngine::GameObject* roundUpButtonObj = RocketEngine::CreateButton("roundUpButtonObj");
	RocketEngine::Button* roundUpButton = roundUpButtonObj->GetComponent<RocketEngine::Button>();

	// 시간 Text
	RocketEngine::GameObject* timeTextObj = RocketEngine::CreateTextBox("timeTextObj");
	RocketEngine::TextBox* timeText = timeTextObj->GetComponent<RocketEngine::TextBox>();
	// 시간 표시 Box
	RocketEngine::GameObject* timeValImageObj = RocketEngine::CreateImageBox("timeValImageObj");
	RocketEngine::SpriteRenderer* timeValSprite = timeValImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	// 시간 Text 표시
	RocketEngine::GameObject* timeValTextObj = RocketEngine::CreateTextBox("timeValTextObj");
	RocketEngine::TextBox* timeValText = timeValTextObj->GetComponent<RocketEngine::TextBox>();
	// 시간 Down 버튼
	RocketEngine::GameObject* timeDownButtonObj = RocketEngine::CreateButton("timeDownButtonObj");
	RocketEngine::Button* timeDownButton = timeDownButtonObj->GetComponent<RocketEngine::Button>();
	// 시간 Up 버튼
	RocketEngine::GameObject* timeUpButtonObj = RocketEngine::CreateButton("timeUpButtonObj");
	RocketEngine::Button* timeUpButton = timeUpButtonObj->GetComponent<RocketEngine::Button>();

	//////////////////////////////////////////////////////////////////////////

	/// Size 지정
	closeButtonObj->GetComponent<RocketEngine::Button>()->SetSize(100.0f, 100.0f);
	roundDownButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);
	roundUpButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);
	timeDownButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);
	timeUpButtonObj->GetComponent<RocketEngine::Button>()->SetSize(70.0f, 70.0f);

	/// 위치 지정
	roomSettingImageObj->transform.SetLocalPosition(0.0f / 2, 575.0f, 0.0f);		// 임시 아직 없음
	closeButtonObj->transform.SetLocalPosition(1219.0f / 2, 270.0f, 0.0f);
	
	roundTextObj->transform.SetLocalPosition(959.0f / 2, 490.0f, 0.0f);	// 아직 없음
	roundValImageObj->transform.SetLocalPosition(959.0f / 2, 590.0f, 0.0f);
	roundValTextObj->transform.SetLocalPosition(959.0f / 2, 590.0f, 0.0f);
	roundDownButtonObj->transform.SetLocalPosition(880.0f / 2, 590.0f, 0.0f);
	roundUpButtonObj->transform.SetLocalPosition(994.0f / 2, 590.0f, 0.0f);

	timeTextObj->transform.SetLocalPosition(959.0f / 2, 653, 0.0f);	// 아직 없음
	timeValImageObj->transform.SetLocalPosition(870.0f / 2, 753.0f, 0.0f);
	timeValTextObj->transform.SetLocalPosition(870.0f / 2, 753.0f, 0.0f);	// 아직 없음
	timeDownButtonObj->transform.SetLocalPosition(822.0f / 2, 753.0f, 0.0f);
	timeUpButtonObj->transform.SetLocalPosition(1097.0f / 2, 753.0f, 0.0f);

	/// Object 기본 상태 설정
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

	/// Text 설정
	roundText->SetText("라운드");
	roundValText->SetText("1");		// 임시
	timeText->SetText("시간");
	timeValText->SetText("0:00");	// 임시

	/// Button 상호작용
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
