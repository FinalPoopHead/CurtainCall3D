#pragma once

#include  <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"


class GameView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	bool Update();

private:
	void testSetup();


protected:
	bool eventFilter(QObject* obj, QEvent* event) override;
	
private slots:
	void handleTabActivated(QDockWidget* dockWidget);
	void pushPlayButton();

private:
	GameView* _gameView;
	Ui::MainWindowClass _ui;

	QPoint m_dragStartPosition;
	QDockWidget* m_activeDockWidget;
};
