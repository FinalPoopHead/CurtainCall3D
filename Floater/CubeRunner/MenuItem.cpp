#include "MenuItem.h"
#include "GameScene.h"


MenuItem::MenuItem()
{
	//_spriteRenderer = AddComponent<flt::RendererComponent>(true);
	_ui = AddComponent<flt::UIComponent>(true);
}

MenuItem::~MenuItem()
{

}

void MenuItem::SetSprite(const std::wstring& spriteName)
{
	_ui->SetImage(spriteName);
}

void MenuItem::SetPosition(flt::Vector2f pos)
{
	_ui->SetPosition(pos);
}

flt::Vector2f MenuItem::GetPosition()
{
	return _ui->GetPosition();
}

void MenuItem::SetSize(flt::Vector2f size)
{
	_ui->SetSize(size);
}

flt::Vector2f MenuItem::GetSize()
{
	return _ui->GetSize();
}

void MenuItem::Select(flt::KeyCode keyCode)
{
	static flt::Scene* gameScene = flt::CreateScene<GameScene>();
	if (keyCode == flt::KeyCode::enter)
	{
		flt::SetScene(gameScene);
	}
}
