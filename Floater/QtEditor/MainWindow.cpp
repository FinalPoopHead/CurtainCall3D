#include "MainWindow.h"
#include "GameView.h"
#include "QDockWidget"

// 테스트용 include.
#include "DockWidget.h"
#include <QTabBar>
#include <QMouseEvent>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, _gameView(nullptr)
{
	ui.setupUi(this);

	/*flt::GameEngine* gameEngine = flt::GameEngine::Instance();
	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");
	_gameView = new GameView(gameEngine, this);

	QDockWidget* dock = new QDockWidget("GameView", this);
	dock->setWidget(_gameView);
	addDockWidget(Qt::LeftDockWidgetArea, dock);*/

	testSetup();
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

void MainWindow::testSetup()
{
	// 솔루션 탐색기
	DockWidget* solutionExplorer = new DockWidget("Solution Explorer", this);
	//solutionExplorer->addTab(new QTextEdit("Solution structure here"), "Solution");
	addDockWidget(Qt::RightDockWidgetArea, solutionExplorer);

	// 속성 창
	DockWidget* properties = new DockWidget("Properties", this);
	//properties->addTab(new QTextEdit("Properties here"), "Properties");
	addDockWidget(Qt::RightDockWidgetArea, properties);

	// 출력 창
	DockWidget* output = new DockWidget("Output", this);
	//output->addTab(new QTextEdit("Output here"), "Output");
	//output->addTab(new QTextEdit("Error List here"), "Error List");
	addDockWidget(Qt::BottomDockWidgetArea, output);

	//// 메인 에디터
	//QTabWidget* centralWidget = new QTabWidget(this);
	//centralWidget->setTabPosition(QTabWidget::North);
	//centralWidget->addTab(new QTextEdit("Main code editor"), "MainWindow.cpp");
	//setCentralWidget(centralWidget);

}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (auto tabBar = qobject_cast<QTabBar*>(obj))
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			auto mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->button() == Qt::LeftButton)
			{
				m_dragStartPosition = mouseEvent->position().toPoint();
			}
		}
		else if (event->type() == QEvent::MouseMove)
		{
			auto mouseEvent = static_cast<QMouseEvent*>(event);
			if (mouseEvent->buttons() & Qt::LeftButton)
			{
				int dragDistance = (mouseEvent->position().toPoint() - m_dragStartPosition).manhattanLength();
				if (dragDistance > QApplication::startDragDistance())
				{
					int index = tabBar->tabAt(m_dragStartPosition);
					if (index != -1 && m_activeDockWidget)
					{
						auto dockWidgets = tabifiedDockWidgets(m_activeDockWidget);
						if (index < dockWidgets.size())
						{
							auto dockWidget = dockWidgets[index];
							dockWidget->setFloating(true);
							dockWidget->move(mapToGlobal(mouseEvent->position().toPoint()));
							return true;
						}
					}
				}
			}
		}
	}
	return QMainWindow::eventFilter(obj, event);
}

void MainWindow::handleTabActivated(QDockWidget* dockWidget)
{
	m_activeDockWidget = dockWidget;
	for (auto child : children())
	{
		if (auto tabBar = qobject_cast<QTabBar*>(child))
		{
			tabBar->installEventFilter(this);
			break;
		}
	}
}
