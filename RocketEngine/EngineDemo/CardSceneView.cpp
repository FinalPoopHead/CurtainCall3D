#include "CardSceneView.h"
#include "RocketAPILoader.h"

#include <cassert>

CardSceneView::CardSceneView()
	: scene()
{

}

void CardSceneView::Initialize()
{
	scene = RocketEngine::CreateScene("CardScene");
	RocketEngine::LoadScene("CardScene");
	assert(scene);

	//////////////////////////////////////////////////////////////////////////
	
	/// Player Info UI
	// Player Info Canvas
	RocketEngine::GameObject* playerInfoCanvas = RocketEngine::CreateObject("playerInfoCanvas");

	// 시간 제한 Image
	RocketEngine::GameObject* timeLimitImageObj = RocketEngine::CreateImageBox("timeLimitImageObj");
	RocketEngine::SpriteRenderer* timeLimitSprite = timeLimitImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* timeLimitText = timeLimitImageObj->GetComponent<RocketEngine::TextBox>();
	timeLimitImageObj->transform.SetLocalPosition(960.0f, 54.0f, 0.0f);
	timeLimitSprite->SetPath("..\\UI\\Card\\ClockBackground.png");
	timeLimitSprite->SetSize(180.0f, 100.0f);
	timeLimitSprite->SetSortOrder(10);
	timeLimitText->SetText("남은 시간 : ");
	timeLimitText->SetSize(180.0f, 100.0f);
	timeLimitText->SetSortOrder(20);

	// 골드 Icon
	RocketEngine::GameObject* goldIconImageObj = RocketEngine::CreateImageBox("goldIconImageObj");
	RocketEngine::SpriteRenderer* goldIconSprite = goldIconImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	goldIconImageObj->transform.SetLocalPosition(861.0f, 171.0f, 0.0f);
	goldIconSprite->SetPath("..\\UI\\Card\\ClockBackground.CoinQuantity\\CoinQuantity.png");
	goldIconSprite->SetSize(100.0f, 100.0f);
	goldIconSprite->SetSortOrder(10);

	// 골드량 
	RocketEngine::GameObject* goldValImageObj = RocketEngine::CreateImageBox("goldValImageObj");
	RocketEngine::SpriteRenderer* goldValSprite = goldValImageObj->GetComponent<RocketEngine::SpriteRenderer>();
	RocketEngine::TextBox* goldValImageText = goldValImageObj->GetComponent<RocketEngine::TextBox>();
	goldValImageObj->transform.SetLocalPosition(1040.0f, 171.0f, 0.0f);
	goldValSprite->SetPath("..\\UI\\Card\\ClockBackground\\CoinQuantity.png");
	goldValSprite->SetSize(180.0f, 100.0f);
	goldValSprite->SetSortOrder(10);
	goldValImageText->SetText("1");
	goldValImageText->SetSize(180.0f, 100.0f);
	goldValImageText->SetSortOrder(20);

	// Canvas Setting
	timeLimitImageObj->transform.SetParent(playerInfoCanvas);
	goldIconImageObj->transform.SetParent(playerInfoCanvas);
	goldValImageObj->transform.SetParent(playerInfoCanvas);

	// Object 기본 상태 설정
	playerInfoCanvas->Enable();

	//////////////////////////////////////////////////////////////////////////

	/// 파츠 강화 창

	// Upgrade Canvas
	RocketEngine::GameObject* upgradeCanvas = RocketEngine::CreateObject("upgradeCanvas");

	// 업그레이드 Box1
	RocketEngine::GameObject* upgradeCardBoxCanvas1 = RocketEngine::CreateImageBox("upgradeCardBoxCanvas1");
	RocketEngine::SpriteRenderer* upgradeCardBoxCanvasSprite1 = upgradeCardBoxCanvas1->GetComponent<RocketEngine::SpriteRenderer>();
	upgradeCardBoxCanvas1->transform.SetLocalPosition(537.0f, 588.0f, 0.0f);
	upgradeCardBoxCanvasSprite1->SetPath("..\\UI\\Card\\ClockBackground\\CardBackGround01.png");
	upgradeCardBoxCanvasSprite1->SetSize(400.0f, 700.0f);
	upgradeCardBoxCanvasSprite1->SetSortOrder(30);

	// 파츠 아이콘 Image1
	RocketEngine::GameObject* partsIconImageObj1 = RocketEngine::CreateImageBox("partsIconImageObj1");
	RocketEngine::SpriteRenderer* partsIconSprite1 = partsIconImageObj1->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconImageObj1->transform.SetLocalPosition(537.0f / 2, 388.0f, 0.0f);
	partsIconSprite1->SetPath("..\\UI\\Card\\ClockBackground\\PartsIcon01.png");
	partsIconSprite1->SetSize(300.0f, 300.0f);
	partsIconSprite1->SetSortOrder(40);

	// 구매 Button1
	RocketEngine::GameObject* buyButtonObj1 = RocketEngine::CreateButton("buyButtonObj1");
	RocketEngine::Button* buyButton1 = buyButtonObj1->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* buyButtonSprite1 = buyButtonObj1->GetComponent<RocketEngine::SpriteRenderer>();
	buyButtonObj1->transform.SetLocalPosition(537.0f, 855.0f, 0.0f);
	buyButton1->SetSize(180.0f, 100.0f);
	buyButton1->SetSortOrder(60);
	buyButtonSprite1->SetPath("..\\UI\\Card\\ClockBackground\\Button01.png");
	buyButtonSprite1->SetSize(180.0f, 100.0f);
	buyButtonSprite1->SetSortOrder(50);

	// 업그레이드 Box2
	RocketEngine::GameObject* upgradeCardBoxCanvas2 = RocketEngine::CreateImageBox("upgradeCardBoxCanvas2");
	RocketEngine::SpriteRenderer* upgradeCardBoxCanvasSprite2 = upgradeCardBoxCanvas2->GetComponent<RocketEngine::SpriteRenderer>();
	upgradeCardBoxCanvas2->transform.SetLocalPosition(959.0f, 588.0f, 0.0f);
	upgradeCardBoxCanvasSprite2->SetPath("..\\UI\\Card\\ClockBackground\\CardBackGround02.png");
	upgradeCardBoxCanvasSprite2->SetSize(400.0f, 700.0f);
	upgradeCardBoxCanvasSprite2->SetSortOrder(30);

	// 파츠 아이콘 Image2
	RocketEngine::GameObject* partsIconImageObj2 = RocketEngine::CreateImageBox("partsIconImageObj2");
	RocketEngine::SpriteRenderer* partsIconSprite2 = partsIconImageObj2->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconImageObj2->transform.SetLocalPosition(951.0f / 2, 388.0f, 0.0f);
	partsIconSprite2->SetPath("..\\UI\\Card\\ClockBackground\\PartsIcon02.png");
	partsIconSprite2->SetSize(300.0f, 300.0f);
	partsIconSprite2->SetSortOrder(40);

	// 구매 Button2
	RocketEngine::GameObject* buyButtonObj2 = RocketEngine::CreateButton("buyButtonObj2");
	RocketEngine::Button* buyButton2 = buyButtonObj2->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* buyButtonSprite2 = buyButtonObj2->GetComponent<RocketEngine::SpriteRenderer>();
	buyButtonObj2->transform.SetLocalPosition(960.0f, 855.0f, 0.0f);
	buyButton2->SetSize(180.0f, 100.0f);
	buyButton2->SetSortOrder(60);
	buyButtonSprite2->SetPath("..\\UI\\Card\\ClockBackground\\Button02.png");
	buyButtonSprite2->SetSize(180.0f, 100.0f);
	buyButtonSprite2->SetSortOrder(50);

	// 업그레이드 Box3
	RocketEngine::GameObject* upgradeCardBoxCanvas3 = RocketEngine::CreateImageBox("upgradeCardBoxCanvas3");
	RocketEngine::SpriteRenderer* upgradeCardBoxCanvasSprite3 = upgradeCardBoxCanvas3->GetComponent<RocketEngine::SpriteRenderer>();
	upgradeCardBoxCanvas3->transform.SetLocalPosition(1381.0f, 588.0f, 0.0f);
	upgradeCardBoxCanvasSprite3->SetPath("..\\UI\\Card\\ClockBackground\\CardBackGround03.png");
	upgradeCardBoxCanvasSprite3->SetSize(400.0f, 700.0f);
	upgradeCardBoxCanvasSprite3->SetSortOrder(30);

	// 파츠 아이콘 Image3
	RocketEngine::GameObject* partsIconImageObj3 = RocketEngine::CreateImageBox("partsIconImageObj3");
	RocketEngine::SpriteRenderer* partsIconSprite3 = partsIconImageObj3->GetComponent<RocketEngine::SpriteRenderer>();
	partsIconImageObj3->transform.SetLocalPosition(1382.0f / 2, 388.0f, 0.0f);
	partsIconSprite3->SetPath("..\\UI\\Card\\ClockBackground\\PartsIcon03.png");
	partsIconSprite3->SetSize(300.0f, 300.0f);
	partsIconSprite3->SetSortOrder(40);

	// 구매 Button3
	RocketEngine::GameObject* buyButtonObj3 = RocketEngine::CreateButton("buyButtonObj3");
	RocketEngine::Button* buyButton3 = buyButtonObj3->GetComponent<RocketEngine::Button>();
	RocketEngine::SpriteRenderer* buyButtonSprite3 = buyButtonObj3->GetComponent<RocketEngine::SpriteRenderer>();
	buyButtonObj3->transform.SetLocalPosition(1382.0f, 855.0f, 0.0f);
	buyButton3->SetSize(180.0f, 100.0f);
	buyButton3->SetSortOrder(60);
	buyButtonSprite3->SetPath("..\\UI\\Card\\ClockBackground\\Button03.png");
	buyButtonSprite3->SetSize(180.0f, 100.0f);
	buyButtonSprite3->SetSortOrder(50);

	// Canvas Setting
	// 업그레이드 개별 창 Canvas
	partsIconImageObj1->transform.SetParent(upgradeCardBoxCanvas1);
	buyButtonObj1->transform.SetParent(upgradeCardBoxCanvas1);
	partsIconImageObj2->transform.SetParent(upgradeCardBoxCanvas2);
	buyButtonObj2->transform.SetParent(upgradeCardBoxCanvas2);
	partsIconImageObj3->transform.SetParent(upgradeCardBoxCanvas3);
	buyButtonObj3->transform.SetParent(upgradeCardBoxCanvas3);

	// 업그레이드 창 묶음
	upgradeCardBoxCanvas1->transform.SetParent(upgradeCanvas);
	upgradeCardBoxCanvas2->transform.SetParent(upgradeCanvas);
	upgradeCardBoxCanvas3->transform.SetParent(upgradeCanvas);

	// Object 기본 상태 설정
	upgradeCanvas->Enable();

	//////////////////////////////////////////////////////////////////////////

	/// Button 설정
	buyButton1->SetOnClickEvent(
		[]()
		{
		
		}
	);

	buyButton2->SetOnClickEvent(
		[]()
		{

		}
	);

	buyButton3->SetOnClickEvent(
		[]()
		{

		}
	);
}
