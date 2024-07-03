#include "MainMenuScene.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuSelector.h"
#include "TestCamera.h"

extern int g_PlayerNum;
extern int g_StageNum;

void MainMenuScene::Initialize()
{
	Menu* menu = flt::CreateGameObject<Menu>(true);

	flt::Vector2f menuItemSize = { 164.0f, 66.4f };
	menuItemSize *= 2.0f;
	float menuItemOffsetX = 0.85f;
	float menuItemOffsetY = 0.65f;

	MenuItem* item1 = flt::CreateGameObject<MenuItem>(true);
	menu->AddItem(item1);

	item1->SetDefaultSpritePath(L"../Resources/Sprites/SinglePlayer2.png");
	item1->SetPointedSpritePath(L"../Resources/Sprites/SinglePlayer2Pointed.png");
	//item1->SetPosition({ 1000.0f, 400.0f });
	item1->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
	menuItemOffsetY += 0.1f;
	item1->SetSize(menuItemSize);

	item1->SetSelectFunc([]() { g_PlayerNum = 1; g_StageNum = 1; flt::SetScene(L"class GameScene"); });


	MenuItem* item2 = flt::CreateGameObject<MenuItem>(true);
	menu->AddItem(item2);

	item2->SetDefaultSpritePath(L"../Resources/Sprites/MultiPlayer2.png");
	item2->SetPointedSpritePath(L"../Resources/Sprites/MultiPlayer2Pointed.png");
	//item2->SetPosition({ 1000.0f, 500.0f });
	item2->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
	menuItemOffsetY += 0.1f;
	item2->SetSize(menuItemSize);

	item2->SetSelectFunc([]() { g_PlayerNum = 2; g_StageNum = 1; flt::SetScene(L"class GameScene"); });

	MenuItem* item3 = flt::CreateGameObject<MenuItem>(true);
	menu->AddItem(item3);

	item3->SetDefaultSpritePath(L"../Resources/Sprites/Exit.png");
	item3->SetPointedSpritePath(L"../Resources/Sprites/ExitPointed.png");
	item3->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
	menuItemOffsetY += 0.1f;
	item3->SetSize(menuItemSize);

	item3->SetSelectFunc([]() { flt::ExitGame(); });

	MenuSelector* selector = flt::CreateGameObject<MenuSelector>(true, menu);


	flt::GameObject* camera = flt::CreateGameObject<flt::GameObject>(true);
	camera->AddComponent<flt::CameraComponent>(true);
}

void MainMenuScene::Finalize()
{

}
