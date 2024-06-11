#include "MenuItem.h"
#include "GameScene.h"


MenuItem::MenuItem()
{
	_spriteRenderer = AddComponent<flt::RendererComponent>(true);
}

MenuItem::~MenuItem()
{

}

void MenuItem::SetSprite(const std::wstring& spriteName)
{
	_spriteRenderer->SetImage(spriteName);
}

void MenuItem::Select(flt::KeyCode keyCode)
{
	static flt::Scene* gameScene = flt::CreateScene<GameScene>();
	if (keyCode == flt::KeyCode::enter)
	{
		flt::SetScene(gameScene);
	}
}
