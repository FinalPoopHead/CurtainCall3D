#include "DockWidget.h"
#include "TabWidget.h"
#include <QTextEdit>

DockWidget::DockWidget(const QString& title, QWidget* parent /*= nullptr*/)
	: QDockWidget(title, parent)
{
	setFeatures(
		QDockWidget::DockWidgetMovable 
		| QDockWidget::DockWidgetFloatable 
		| QDockWidget::DockWidgetClosable
	);

	QTextEdit* textEdit1 = new QTextEdit(this);
	setWidget(textEdit1);

	//setTitleBarWidget(new QWidget());

	//_tabWidget = new TabWidget(this);
	//setWidget(_tabWidget);
}

//void DockWidget::addTab(QWidget* widget, const QString& label)
//{
//	_tabWidget->addTab(widget, label);
//}
