#pragma once

#include <QtWidgets/qwidget.h>
//#include "ui_GameView.h"
#include "../FloaterUtil/include/FloaterType.h"

class QLabel;

namespace flt
{
	class Platform;
}

class GameView : public QWidget
{
	Q_OBJECT

public:
	GameView(flt::Platform* winPlayform, QWidget* parent = nullptr);
	~GameView();

	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent *event) override;


private:
	void CloseNativeWindow();

private:
	flt::Platform* _winPlatform;
	uint64 _hwnd;
	QWidget* _container;

private:
	constexpr static int TITLE_HEIGHT = 20;
	constexpr static int BORDER_WIDTH = 1;
};
