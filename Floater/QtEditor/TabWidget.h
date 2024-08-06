#pragma once
#include <QTabWidget>

class TabWidget : public QTabWidget
{
public:
	TabWidget(QWidget* parent = nullptr);

protected:
	//void mousePressEvent(QMouseEvent* event) override;
	//void mouseMoveEvent(QMouseEvent* event) override;

private:
	QPoint _dragStartPosition;
};
