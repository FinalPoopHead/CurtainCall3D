#include "MainWindow.h"
#include "GameView.h"
#include "QDockWidget"

// 테스트용 include.
#include <QTabBar>
#include <QMouseEvent>
#include <QDockWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, _gameView(nullptr)
	, _dockManager(nullptr)
{
	_ui.setupUi(this);

	ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
	ads::CDockManager::setConfigFlag(ads::CDockManager::ActiveTabHasCloseButton, true);
	ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasCloseButton, false);
	ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHasUndockButton, false);
	ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaDynamicTabsMenuButtonVisibility, true);
	ads::CDockManager::setConfigFlag(ads::CDockManager::OpaqueSplitterResize, true);
	//만약 나중에 위젯 하나를 독매니저 겸 도킹 스테이션으로 쓸려 할 때 
	//ads::CDockManager::setConfigFlag(ads::CDockManager::HideSingleCentralWidgetTitleBar, true);

	ads::CDockManager::setConfigFlag(ads::CDockManager::DragPreviewIsDynamic, true);
	ads::CDockManager::setConfigFlag(ads::CDockManager::DragPreviewShowsContentPixmap, false);
	ads::CDockManager::setConfigFlag(ads::CDockManager::DragPreviewHasWindowFrame, false);
	ads::CDockManager::setConfigFlag(ads::CDockManager::FloatingContainerHasWidgetIcon, true);

	ads::CDockManager::setAutoHideConfigFlags(ads::CDockManager::DefaultAutoHideConfig);
	
	_dockManager = new ads::CDockManager(this);
	_dockManager->setDockWidgetToolBarStyle(Qt::ToolButtonIconOnly, ads::CDockWidget::StateFloating);

	resize(1024, 768);

	// Menu -> File
	connect(_ui.actionExit, &QAction::triggered, this, &QMainWindow::close);

	// Menu -> View
	connect(_ui.actionScene, &QAction::triggered, this, &MainWindow::handleSceneView);
	connect(_ui.actionGame, &QAction::triggered, this, &MainWindow::handleGameView);
	connect(_ui.actionOutput, &QAction::triggered, this, &MainWindow::handleOutputView);

	//connect(_ui.playButton, &QPushButton::clicked, this, &MainWindow::pushPlayButton);

	//testSetup();
}

MainWindow::~MainWindow()
{
	delete _gameView;

}

bool MainWindow::Update()
{
	if (_gameView)
	{
		_gameView->gameUpdate();
	}
	return true;
}

void MainWindow::testSetup()
{
	// 솔루션 탐색기
	ads::CDockWidget* solutionExplorer = new ads::CDockWidget("Solution Explorer", this);
	solutionExplorer->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	_dockManager->addDockWidget(ads::LeftDockWidgetArea, solutionExplorer);

	// 속성 창
	ads::CDockWidget* Properties = new ads::CDockWidget("Properties", this);
	Properties->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	_dockManager->addDockWidget(ads::LeftDockWidgetArea, Properties);

	// 출력 창
	ads::CDockWidget* Output = new ads::CDockWidget("Output", this);
	Output->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	_dockManager->addDockWidget(ads::LeftDockWidgetArea, Output);

	// 게임 뷰
	ads::CDockWidget* GameView = new ads::CDockWidget("GameView", this);
	GameView->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	_dockManager->addDockWidget(ads::LeftDockWidgetArea, GameView);

	// 씬 뷰
	ads::CDockWidget* SceneView = new ads::CDockWidget("SceneView", this);
	SceneView->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	_dockManager->addDockWidget(ads::LeftDockWidgetArea, SceneView);

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

void MainWindow::pushPlayButton()
{
	if (_gameView == nullptr)
	{

	}

	if (_gameView->isPlaying())
	{
		_gameView->stop();
	}
	else
	{
		_gameView->run();
	}
}

void MainWindow::handleSceneView(bool checked)
{
	if(checked)
	{
		_sceneViewDock = new ads::CDockWidget("SceneView", this);
		//_sceneViewDock->setFeature(ads::CDockWidget::DockWidgetClosable, true);
		_sceneViewDock->resize(QSize{640, 480});
		_dockManager->addDockWidgetFloating(_sceneViewDock);
	}
	else
	{
		_dockManager->removeDockWidget(_sceneViewDock);
	}
}

void MainWindow::handleGameView(bool checked)
{
	if (checked)
	{
		_gameViewDock = new ads::CDockWidget("GameView", this);
		//_gameViewDock->setFeature(ads::CDockWidget::DockWidgetClosable, true);
		_gameView = new GameView(_gameViewDock);
		_gameViewDock->setWidget(_gameView);
		_gameViewDock->resize(640, 480);
		_dockManager->addDockWidgetFloating(_gameViewDock); 
	}
	else
	{
		_dockManager->removeDockWidget(_gameViewDock);
		_gameView = nullptr;
	}
}

void MainWindow::handleOutputView(bool checked)
{
	if (checked)
	{
		_outputViewDock = new ads::CDockWidget("OutputView", this);
		//_outputViewDock->setFeature(ads::CDockWidget::DockWidgetClosable, true);
		_outputViewDock->resize(640, 480);
		_dockManager->addDockWidgetFloating(_outputViewDock);
	}
	else
	{
		_dockManager->removeDockWidget(_outputViewDock);
	}
}
