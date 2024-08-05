#pragma once

#include <QtWidgets/qwidget.h>
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
	GameView(flt::GameEngine* gameEngine, QWidget* parent = nullptr);
	~GameView();

	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent *event) override;

	bool gameUpdate(const QByteArray& eventType, void* message, long* result);
private:
	void ChangeWindowMaximize();
	void CloseNativeWindow();

private:
	flt::GameEngine* _gameEngine;
	uint64 _hwnd;
	QWidget* _container;

private:
	constexpr static int TITLE_HEIGHT = 20;
	constexpr static int BORDER_WIDTH = 1;

	bool _isRunning;
};
