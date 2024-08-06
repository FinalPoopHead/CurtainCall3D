#include "GameView.h"
#include <QAbstractEventDispatcher>
#include <QtGui/QWindow>
#include <QVBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QPainter>
#include <QStyleFactory>
#include <QTimer>
#include <QApplication>
#include <windows.h>
#include <iostream>

#include "../FloaterGameEngine/include/internal/GameEngine.h"
#include "../FloaterPlatform/include/Platform.h"
#include "../FloaterGameEngine/include/EngineMinimal.h"
#include "./test/GameScene.h"
#include "./test/MainMenuScene.h"
#include "TitleBar.h"

const QEvent::Type CustomEventType = static_cast<QEvent::Type>(QEvent::User + 1);

class CustomEvent : public QEvent
{
public:
	CustomEvent() : QEvent(CustomEventType) {}
};


GameView::GameView(QWidget* parent /*= nullptr*/) : QWidget(parent)
	, _gameEngine(flt::GameEngine::Instance())
	, _hwnd(NULL)
	, _layout(nullptr)
	, _container(nullptr)
	, _timer(new QTimer(this))
	, _isRunning(false) // 일단은 True로 해서 반드시 게임이 돌아가도록.
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	//setAttribute(Qt::WA_TranslucentBackground);

	_layout = new QVBoxLayout(this);
	_layout->setContentsMargins(BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH, BORDER_WIDTH);
	_layout->setSpacing(0);
	setLayout(_layout);

	resize(800, 600);

	//connect(titleBar, &TitleBar::minimizeClicked, this, &QWidget::showMinimized);
	//connect(titleBar, &TitleBar::maximizeClicked, this, &GameView::ChangeWindowMaximize);
	//connect(titleBar, &TitleBar::closeClicked, this, &QWidget::close);

	connect(_timer, &QTimer::timeout, this, &GameView::performTask);
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
		_container->setGeometry(BORDER_WIDTH, BORDER_WIDTH,
			width() - 2 * BORDER_WIDTH,
			height() - 2 * BORDER_WIDTH);
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

void GameView::performTask()
{
	QApplication::processEvents();
}

void GameView::gameUpdate()
{
	if (_isRunning)
	{
		if (!_gameEngine->Update())
		{
			stop();
			return;
		}
	}

	return;
}

bool GameView::isPlaying()
{
	return _isRunning;
}

void GameView::run()
{
	if(_isRunning)
	{
		return;
	}

	_isRunning = true;
	_gameEngine->Initialize();

	_hwnd = _gameEngine->GetWindowHandle();
	_gameEngine->GetPlatform()->ShowCursor(true);


	flt::CreateScene<MainMenuScene>();
	flt::CreateScene<GameScene>();
	flt::SetScene(L"class MainMenuScene");

	_container = QWidget::createWindowContainer(QWindow::fromWinId((WId)_hwnd), this);

	_layout->addWidget(_container, 0);

	QAbstractEventDispatcher* dispatcher = QAbstractEventDispatcher::instance();
	_connection = connect(dispatcher, &QAbstractEventDispatcher::aboutToBlock, this, &GameView::gameUpdate);
	_timer->start(1);
}

void GameView::stop()
{
	if(!_isRunning)
	{
		return;
	}

	_timer->stop();
	disconnect(_connection);
	_connection = QMetaObject::Connection();
	_isRunning = false;

	_layout->removeWidget(_container);
	_container->setParent(nullptr);
	delete _container;
	_container = nullptr;
	_hwnd = NULL;
	_gameEngine->Finalize();
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
