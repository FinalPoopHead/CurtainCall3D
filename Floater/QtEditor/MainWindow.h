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
	GameView* _gameView;
	Ui::MainWindowClass ui;
};
