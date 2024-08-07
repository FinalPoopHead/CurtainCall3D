#pragma once

#include  <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

// QtAdvancedDockingSystem
#include "./ads/DockManager.h"
#include "./ads/DockWidget.h"
#include "./ads/DockAreaWidget.h"
#include "./ads/DockAreaTitleBar.h"
#include "./ads/DockAreaTabBar.h"
#include "./ads/FloatingDockContainer.h"
#include "./ads/DockComponentsFactory.h"
#include "./ads/DockSplitter.h"

#ifdef _DEBUG
#pragma comment(lib, "../External/lib/x64/debug/qtadvanceddockingd.lib")
#else
#pragma comment(lib, "../External/lib/x64/release/qtadvanceddocking.lib")
#endif

class GameView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	bool Update();

private:
	void testSetup();

protected:
	bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
	void handleTabActivated(QDockWidget* dockWidget);
	void pushPlayButton();

	void handleSceneView(bool checked);
	void handleGameView(bool checked);
	void handleOutputView(bool checked);

private:
	Ui::MainWindowClass _ui;

	GameView* _gameView;
	ads::CDockWidget* _gameViewDock;
	ads::CDockWidget* _sceneViewDock;
	ads::CDockWidget* _outputViewDock;


	QPoint m_dragStartPosition;
	QDockWidget* m_activeDockWidget;

	ads::CDockManager* _dockManager;
};
