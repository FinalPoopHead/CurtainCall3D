#include "ViewWrapWidget.h"

ViewWrapWidget::ViewWrapWidget(const QString& title, QWidget* view, QWidget* parent /*= nullptr*/)
	: ads::CDockWidget(title, parent)
{
	view->setParent(this);
	setWidget(view);
	resize(640, 480);
}

ViewWrapWidget::~ViewWrapWidget()
{

}

void ViewWrapWidget::closeEvent(QCloseEvent* event)
{
	emit widgetClosed();
	ads::CDockWidget::closeEvent(event);
}
