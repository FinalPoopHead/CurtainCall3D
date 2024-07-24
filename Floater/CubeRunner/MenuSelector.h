#pragma once
#include "../FloaterGameEngine/include/EngineMinimal.h"


class Menu;
class MenuItem;
class RankViewer;

class MenuSelector : public flt::GameObject
{
	enum class Mode
	{
		MainMenu,
		RankView,
		ControllerSelect
	};
public:
	MenuSelector(Menu* mainMenu, Menu* controllerMenu);
	void SetMenu(Menu* mainMenu);
	void next();
	void prev();
	void Select(flt::KeyCode keyCode);

	void SetTitle(MenuItem* title) { _title = title; }
	void SetRankViewer(RankViewer* rankViewer) { _rankViewer = rankViewer; }

	void SetMainMenuMode();
	void SetViewRankMode();
	void SetControllerSelectMode();

protected:
	void OnEnable() override;
	void OnDisable() override;
	void Update(float deltaSecond) override;

private:
	void MoveSelectedItem();


private:
	MenuItem* _title;
	Menu* _mainMenu;
	Menu* _controllerSelectMenu;
	RankViewer* _rankViewer;
	MenuItem* _selectedItem;
	flt::UIComponent* _ui;
	float _lastLStickY[2];
	Mode _mode;
};
