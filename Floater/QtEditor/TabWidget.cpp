#include "TabWidget.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>

TabWidget::TabWidget(QWidget* parent /*= nullptr*/)
	: QTabWidget(parent)
{
	setTabPosition(QTabWidget::North);
	setMovable(true);
	setTabsClosable(true);
}

//void TabWidget::mousePressEvent(QMouseEvent* event)
//{
//	if (event->button() == Qt::LeftButton) 
//	{
//		_dragStartPosition = event->pos();
//	}
//	QTabWidget::mousePressEvent(event);
//}
//
//void TabWidget::mouseMoveEvent(QMouseEvent* event)
//{
//	if (!(event->buttons() & Qt::LeftButton)) 
//	{
//		return;
//	}
//	if ((event->pos() - _dragStartPosition).manhattanLength() < QApplication::startDragDistance()) 
//	{
//		return;
//	}
//
//	QDrag* drag = new QDrag(this);
//	QMimeData* mimeData = new QMimeData;
//	mimeData->setText(tabText(currentIndex()));
//	drag->setMimeData(mimeData);
//
//	Qt::DropAction dropAction = drag->exec(Qt::MoveAction);
//}
