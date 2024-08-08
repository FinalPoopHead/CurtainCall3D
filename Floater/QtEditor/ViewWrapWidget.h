#pragma once
#include "./ads/DockWidget.h"

class ViewWrapWidget : public ads::CDockWidget
{
	Q_OBJECT

public:
	ViewWrapWidget(const QString& title, QWidget* view, QWidget* parent = nullptr);
	~ViewWrapWidget();

protected:
	void closeEvent(QCloseEvent* event) override;

signals:
	void widgetClosed();

private:
};

