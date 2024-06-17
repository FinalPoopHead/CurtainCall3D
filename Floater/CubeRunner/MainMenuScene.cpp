#include "MainMenuScene.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuSelector.h"
#include "TestCamera.h"


void MainMenuScene::Initialize()
{
	Menu* menu = flt::CreateGameObject<Menu>(true);

	MenuItem* item1 = flt::CreateGameObject<MenuItem>(true);
	menu->AddItem(item1);

	item1->SetSprite(L"../Resources/Sprites/abcd.jpg");
	//item1->SetPosition({ 1000.0f, 400.0f });
	item1->SetOffsetPosition({ 0.75f, 0.65f });
	item1->SetSize({100.0f, 80.0f});

	item1->SetSelectFunc([]() { flt::SetScene(L"class GameScene"); });


	MenuItem* item2 = flt::CreateGameObject<MenuItem>(true);
	menu->AddItem(item2);

	item2->SetSprite(L"../Resources/Sprites/test.jpg");
	//item2->SetPosition({ 1000.0f, 500.0f });
	item2->SetOffsetPosition({ 0.75f, 0.8f });
	item2->SetSize({ 100.0f, 80.0f });

	item2->SetSelectFunc([]() { flt::ExitGame(); });

	MenuItem* item3 = flt::CreateGameObject<MenuItem>(true);
	menu->AddItem(item3);

	item3->SetText(L"Test");
	item3->SetFont(L"../Resources/Fonts/NotoSansKR.spritefont");
	item3->SetTextColor(1.0f, 0.0f, 0.0f);
	item3->SetOffsetPosition({ 0.75f, 0.5f });


	MenuSelector* selector = flt::CreateGameObject<MenuSelector>(true, menu);


	flt::GameObject* camera = flt::CreateGameObject<flt::GameObject>(true);
	camera->AddComponent<flt::CameraComponent>(true);

}

void MainMenuScene::Finalize()
{

}
