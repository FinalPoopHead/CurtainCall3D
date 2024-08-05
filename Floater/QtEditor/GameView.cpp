#include "GameView.h"
#include <QtGui/QWindow>
#include <QVBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QPainter>
#include <QStyleFactory>
#include <windows.h>
#include <iostream>

#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "../FloaterPlatform/include/Platform.h"
#include "TitleBar.h"

GameView::GameView(flt::GameEngine* gameEngine, QWidget* parent /*= nullptr*/) : QWidget(parent)
	, _gameEngine(gameEngine)
	, _hwnd(NULL)
	, _container(nullptr)
	, _isRunning(true) // 일단은 True로 해서 반드시 게임이 돌아가도록.
{
	_hwnd = gameEngine->GetWindowHandle();
	gameEngine->GetPlatform()->ShowCursor(true);

	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	//setAttribute(Qt::WA_TranslucentBackground);

	_container = QWidget::createWindowContainer(QWindow::fromWinId((WId)_hwnd), this);

	TitleBar* titleBar = new TitleBar("GameView", this);
	QWidget* centralWidget = new QWidget(this);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH);
	layout->setSpacing(0);
	layout->addWidget(titleBar, 0);
	layout->addWidget(_container, 1);
	setLayout(layout);

	resize(800, 600);

	connect(titleBar, &TitleBar::minimizeClicked, this, &QWidget::showMinimized);
	connect(titleBar, &TitleBar::maximizeClicked, this, &GameView::ChangeWindowMaximize);
	connect(titleBar, &TitleBar::closeClicked, this, &QWidget::close);
}


GameView::~GameView()
{
	CloseNativeWindow();
}

void GameView::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	if(_hwnd != NULL)
	{
		//int w = width();
		//int h = height();
		//::SetWindowPos((HWND)_hwnd, NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOMOVE);

		//_titleLabel->setFixedSize(width() - 2 * BORDER_WIDTH, TITLE_HEIGHT);
		_container->setGeometry(BORDER_WIDTH, TITLE_HEIGHT + BORDER_WIDTH,
			width() - 2 * BORDER_WIDTH,
			height() - TITLE_HEIGHT - 2 * BORDER_WIDTH);
		SetWindowPos((HWND)_hwnd, NULL, 0, 0,
			_container->width(), _container->height(),
			SWP_NOZORDER | SWP_NOMOVE);
	}
}

void GameView::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);

	QPainter painter(this);
	painter.setPen(QPen(QColor("#3C3C3C"), 1));
	painter.drawRect(rect().adjusted(0, 0, -1, -1));
}

void GameView::closeEvent(QCloseEvent* event)
{
	QWidget::closeEvent(event);
	
	if(_hwnd != NULL)
	{
		CloseNativeWindow();
	}
}

bool GameView::gameUpdate(const QByteArray& eventType, void* message, long* result)
{
	if (_isRunning)
	{
		return true;
	}

	return true;
}

void GameView::ChangeWindowMaximize()
{
	if (isMaximized())
		QWidget::showNormal();
	else
		QWidget::showMaximized();
}

void GameView::CloseNativeWindow()
{
	if (IsWindow((HWND)_hwnd))
	{
		::PostMessage((HWND)_hwnd, WM_CLOSE, 0, 0);
	}
}
