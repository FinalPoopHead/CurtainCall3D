#pragma once
#include <QWidget>
//#include <QDockWidget>
#include <QTabWidget>
//#include "ui_GameView.h"
#include "../FloaterUtil/include/FloaterType.h"

class QLabel;
class QVBoxLayout;
class QTimer;
class QMetaObject;
class QMetaObject::Connection;

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

	bool isPlaying();
	void run();
	void stop();

public slots:
	void performTask();
	void gameUpdate();

private:
	void ChangeWindowMaximize();
	void CloseNativeWindow();

private:
	// 게임 Play관련 변수
	flt::GameEngine* _gameEngine;
	uint64 _hwnd;
	QVBoxLayout* _layout;
	QWidget* _container;
	QTimer* _timer;
	QMetaObject::Connection _connection;
	bool _isRunning;

private:
	constexpr static int TITLE_HEIGHT = 20;
	constexpr static int BORDER_WIDTH = 1;
};
