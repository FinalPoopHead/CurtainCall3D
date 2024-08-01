#define _CRT_SECURE_NO_WARNINGS
#include <QtWidgets/QApplication>
#include <QtGui/QWindow>
#include "MainWindow.h"
#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "../FloaterGameEngine/include/Input.h"
#include "./test/GameScene.h"
#include "./test/MainMenuScene.h"
#include "GameView.h"


int main(int argc, char* argv[])
{
	
	//MainWindow w;
	//w.show();
	//auto hwnd = w.winId();

	flt::GameEngine* gameEngine = flt::GameEngine::Instance();

	QApplication a(argc, argv);
	//QWidget* container = QWidget::createWindowContainer(QWindow::fromWinId((WId)gameEngine->GetWindowHandle()));
	//container->setAttribute(Qt::WA_DontCreateNativeAncestors);
	//container->setAttribute(Qt::WA_NativeWindow);
	//container->setAttribute(Qt::WA_InputMethodEnabled, false);
	//container->show();
	GameView gameView(gameEngine->GetPlatform());
	gameView.show();

	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");

	FILE* console = freopen("CONOUT$", "w", stdout);
	ASSERT(console, "Failed to open console window");
	std::cout.clear();

	while (true)
	{
		if (!gameEngine->Update())
		{
			gameView.close();
			break;
		}
		a.processEvents();
	}

	gameEngine->Finalize();
	fclose(console);

	//return a.exec();
}
