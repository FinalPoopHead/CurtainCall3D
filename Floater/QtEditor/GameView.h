#pragma once
#include <QWidget>
//#include <QDockWidget>
#include <QTabWidget>
//#include "ui_GameView.h"
#include "../FloaterUtil/include/FloaterType.h"

class QLabel;

namespace flt
{
	//class Platform;
	class GameEngine;
}

class GameView : public QWidget
{
	Q_OBJECT

public:
	GameView(QWidget* parent = nullptr);
	~GameView();

	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent *event) override;

	bool gameUpdate();

	void run();
	void stop();
private:
	void ChangeWindowMaximize();
	void CloseNativeWindow();

private:
	flt::GameEngine* _gameEngine;
	uint64 _hwnd;
	QWidget* _container;

	bool _isRunning;
private:
	constexpr static int TITLE_HEIGHT = 20;
	constexpr static int BORDER_WIDTH = 1;
};
