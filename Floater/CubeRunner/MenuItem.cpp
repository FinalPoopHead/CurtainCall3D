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

void MenuItem::SetPosition(float pixelX, float pixelY)
{
	_ui->SetPosition(pixelX, pixelY);
}

void MenuItem::SetSize(float pixelWidth, float pixelHeight)
{
	_ui->SetSize(pixelWidth, pixelHeight);
}

void MenuItem::GetSize(float& pixelWidth, float& pixelHeight)
{
	_ui->GetSize(pixelWidth, pixelHeight);
}

void MenuItem::Select(flt::KeyCode keyCode)
{
	static flt::Scene* gameScene = flt::CreateScene<GameScene>();
	if (keyCode == flt::KeyCode::enter)
	{
		flt::SetScene(gameScene);
	}
}
