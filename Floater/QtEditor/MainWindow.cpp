#include "MainWindow.h"
#include "GameView.h"
#include "QDockWidget"

// 테스트용 include.
#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "./test/GameScene.h"
#include "./test/MainMenuScene.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, _gameView(nullptr)
{
	ui.setupUi(this);

	flt::GameEngine* gameEngine = flt::GameEngine::Instance();
	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");
	_gameView = new GameView(gameEngine, this);

	QDockWidget* dock = new QDockWidget("GameView", this);
	dock->setWidget(_gameView);
	addDockWidget(Qt::LeftDockWidgetArea, dock);
}

MainWindow::~MainWindow()
{
	delete _gameView;

}

bool MainWindow::Update()
{
	if (_gameView)
	{
		return _gameView->gameUpdate();
	}
	return true;
}
