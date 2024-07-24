#include "MainMenuScene.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuSelector.h"
#include "TestCamera.h"
#include "RankViewer.h"

extern int g_PlayerNum;
extern int g_StageNum;

extern int g_player1Input;
extern int g_player2Input;

void MainMenuScene::Initialize()
{
	Menu* menu = flt::CreateGameObject<Menu>(true);
	Menu* controllerMenu = flt::CreateGameObject<Menu>(false);
	MenuSelector* selector = flt::CreateGameObject<MenuSelector>(true, menu, controllerMenu);

	MenuItem* title = flt::CreateGameObject<MenuItem>(true);
	title->SetDefaultSpritePath(L"../Resources/Sprites/GameTitle.png");
	title->SetOffsetPosition({ 0.5f, 0.25f });
	flt::Vector2f titleSize{ 256.0f, 122.8f };
	titleSize *= 4.f;
	title->SetSize(titleSize);
	flt::Vector2f titlePos = title->GetOffsetPosition();
	auto titleTween = flt::MakeTween(flt::Vector2f{ titlePos.x, 0.0f });
	titleTween->to({ titlePos.x, titlePos.y }).during(1.0f).easing(flt::ease::easeOutElastic).onStep([title](const flt::Vector2f& pos) {title->SetOffsetPosition(pos); });
	title->SetTween(std::move(titleTween));

	selector->SetTitle(title);

	{
		flt::Vector2f menuItemSize = { 164.0f, 66.4f };
		menuItemSize *= 4.0f;
		float menuItemOffsetX = 0.85f;
		float menuItemOffsetY = 0.55f;
		constexpr uint32 numMenuItems = 4;

		std::wstring defaultSpritePath[numMenuItems] = {
			L"../Resources/Sprites/SinglePlayer2.png",
			L"../Resources/Sprites/MultiPlayer2.png",
			L"../Resources/Sprites/Rank.png",
			L"../Resources/Sprites/Exit.png"
		};
		std::wstring pointedSpritePath[numMenuItems] = {
			L"../Resources/Sprites/SinglePlayer2Pointed.png",
			L"../Resources/Sprites/MultiPlayer2Pointed.png",
			L"../Resources/Sprites/RankPointed.png",
			L"../Resources/Sprites/ExitPointed.png"
		};
		std::function<void()> selectFunc[numMenuItems] = {
			[]() { g_PlayerNum = 1; g_StageNum = 1; flt::SetScene(L"class GameScene"); },
			[]() { g_PlayerNum = 2; g_StageNum = 1; flt::SetScene(L"class GameScene"); },
			[selector]() { selector->SetViewRankMode(); },
			[]() { flt::ExitGame(); }
		};

		for (int i = 0; i < numMenuItems; ++i)
		{
			MenuItem* item = flt::CreateGameObject<MenuItem>(false);
			menu->AddItem(item);
			item->SetDefaultSpritePath(defaultSpritePath[i]);
			item->SetPointedSpritePath(pointedSpritePath[i]);
			item->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
			menuItemOffsetY += 0.1f;
			item->SetSize(menuItemSize);
			item->SetSelectFunc(selectFunc[i]);
			flt::Vector2f pos = item->GetOffsetPosition();
			auto tween = flt::MakeTween(flt::Vector2f{ 1.0f, pos.y });
			tween->to({ pos.x, pos.y }).during(1.0f).easing(flt::ease::easeOutElastic).onStep([item](const flt::Vector2f& pos) {item->SetOffsetPosition(pos); });
			item->SetTween(std::move(tween));
		}

		/*MenuItem* item1 = flt::CreateGameObject<MenuItem>(true);
		menu->AddItem(item1);
		item1->SetDefaultSpritePath(L"../Resources/Sprites/SinglePlayer2.png");
		item1->SetPointedSpritePath(L"../Resources/Sprites/SinglePlayer2Pointed.png");
		item1->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
		menuItemOffsetY += 0.1f;
		item1->SetSize(menuItemSize);
		item1->SetSelectFunc([]() { g_PlayerNum = 1; g_StageNum = 1; flt::SetScene(L"class GameScene"); });
		flt::Vector2f pos = item1->GetOffsetPosition();
		auto tween = flt::MakeTween(flt::Vector2f{ 1.0f, pos.y });
		tween->to({ pos.x, pos.y }).during(1.0f).easing(flt::ease::easeOutElastic).onStep([item1](const flt::Vector2f& pos) {item1->SetOffsetPosition(pos); });
		item1->SetTween(std::move(tween));


		MenuItem* item2 = flt::CreateGameObject<MenuItem>(true);
		menu->AddItem(item2);
		item2->SetDefaultSpritePath(L"../Resources/Sprites/MultiPlayer2.png");
		item2->SetPointedSpritePath(L"../Resources/Sprites/MultiPlayer2Pointed.png");
		item2->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
		menuItemOffsetY += 0.1f;
		item2->SetSize(menuItemSize);
		item2->SetSelectFunc([]() { g_PlayerNum = 2; g_StageNum = 1; flt::SetScene(L"class GameScene"); });

		MenuItem* item3 = flt::CreateGameObject<MenuItem>(true);
		menu->AddItem(item3);
		item3->SetDefaultSpritePath(L"../Resources/Sprites/Rank.png");
		item3->SetPointedSpritePath(L"../Resources/Sprites/RankPointed.png");
		item3->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
		menuItemOffsetY += 0.1f;
		item3->SetSize(menuItemSize);
		item3->SetSelectFunc([selector]() { selector->ViewRank(); });

		MenuItem* item4 = flt::CreateGameObject<MenuItem>(true);
		menu->AddItem(item4);
		item4->SetDefaultSpritePath(L"../Resources/Sprites/Exit.png");
		item4->SetPointedSpritePath(L"../Resources/Sprites/ExitPointed.png");
		item4->SetOffsetPosition({ menuItemOffsetX, menuItemOffsetY });
		menuItemOffsetY += 0.1f;
		item4->SetSize(menuItemSize);
		item4->SetSelectFunc([]() { flt::ExitGame(); });*/
	}

	{
		MenuItem* item1 = flt::CreateGameObject<MenuItem>(false);
		controllerMenu->AddItem(item1);

		item1->SetDefaultSpritePath(L"../Resources/Sprites/abcd.jpg");
		item1->SetPointedSpritePath(L"../Resources/Sprites/test.jpg");
		item1->SetOffsetPosition({ 0.25f, 0.5f });
		item1->SetSize({ 500.0f, 500.0f });

		item1->SetSelectFunc([]() {});

		MenuItem* item2 = flt::CreateGameObject<MenuItem>(false);
		controllerMenu->AddItem(item2);

		item2->SetDefaultSpritePath(L"../Resources/Sprites/abcd.jpg");
		item2->SetPointedSpritePath(L"../Resources/Sprites/test.jpg");
		item2->SetOffsetPosition({ 0.75f, 0.5f });
		item2->SetSize({ 500.0f, 500.0f });

		item2->SetSelectFunc([]() {});
	}

	RankViewer* rankViewer = flt::CreateGameObject<RankViewer>(false);

	selector->SetRankViewer(rankViewer);


	flt::GameObject* camera = flt::CreateGameObject<flt::GameObject>(true);
	camera->AddComponent<flt::CameraComponent>(true);

	flt::GameObject* bgmObject = flt::CreateGameObject<flt::GameObject>(true);
	bgmObject->AddComponent<flt::SoundComponent>(true);
	bgmObject->GetComponent<flt::SoundComponent>()->AddSound(L"../Resources/Sound/menu/universe-space-sounds-3595.mp3");
	bgmObject->GetComponent<flt::SoundComponent>()->Play(0, true);
}

void MainMenuScene::Finalize()
{

}
