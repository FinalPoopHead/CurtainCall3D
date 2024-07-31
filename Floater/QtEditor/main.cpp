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


int main(int argc, char* argv[])
{
	
	//MainWindow w;
	//w.show();
	//auto hwnd = w.winId();

	flt::GameEngine* gameEngine = flt::GameEngine::Instance();

	QApplication a(argc, argv);
	QWidget* container = QWidget::createWindowContainer(QWindow::fromWinId((WId)gameEngine->GetWindowHandle()));
	container->setAttribute(Qt::WA_DontCreateNativeAncestors);
	container->setAttribute(Qt::WA_NativeWindow);
	container->setAttribute(Qt::WA_InputMethodEnabled, false);
	container->show();

	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");

	freopen("CONOUT$", "w", stdout);
	std::cout.clear();

	bool isRun = true;
	while (isRun)
	{
		isRun = gameEngine->Update();
		a.processEvents();

		if (flt::GetKeyDown(flt::KeyCode::esc))
		{
			std::cout << "ESC" << std::endl;
		}

		if(flt::GetKeyDown(flt::KeyCode::right))
		{
			std::cout << "Right Down" << std::endl;
		}

		//if(flt::GetKey(flt::KeyCode::right))
		//{
		//	std::cout << "Right" << std::endl;
		//}

		if(flt::GetKeyUp(flt::KeyCode::right))
		{
			std::cout << "Right Up" << std::endl;
		}

	}

	gameEngine->Finalize();

	//return a.exec();
}
