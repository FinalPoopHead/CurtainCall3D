#include "MainMenuScene.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuSelector.h"
#include "TestCamera.h"


void MainMenuScene::Initialize()
{
	Menu* menu = flt::CreateGameObject<Menu>(true);

	MenuItem* item1 = flt::CreateGameObject<MenuItem>(true);
	item1->SetSprite(L"../Resources/Textures/abcd.jpg");
	item1->SetSize(100.0f, 100.0f);
	menu->AddItem(item1);

	MenuItem* item2 = flt::CreateGameObject<MenuItem>(true);
	item2->SetSprite(L"../Resources/Textures/test.jpg");
	item2->SetPosition(100.0f, 100.0f);
	item2->SetSize(100.0f, 100.0f);
	menu->AddItem(item2);

	MenuSelector* selector = flt::CreateGameObject<MenuSelector>(true, menu);


	flt::GameObject* camera = flt::CreateGameObject<flt::GameObject>(true);
	camera->AddComponent<flt::CameraComponent>(true);

}

void MainMenuScene::Finalize()
{

}
