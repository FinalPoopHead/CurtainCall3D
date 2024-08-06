#pragma once
#include <QDockWidget>

class TabWidget;

class DockWidget : public QDockWidget
{
public:
	DockWidget(const QString& title, QWidget* parent = nullptr);

	//void addTab(QWidget* widget, const QString& label);

private:
	//TabWidget* _tabWidget;
};
