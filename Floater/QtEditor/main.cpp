#define _CRT_SECURE_NO_WARNINGS
#include "EditorApplication.h"
#include "MainWindow.h"
#include <QtGui/QWindow>
#include <QtWidgets/QApplication>
#include <iostream>
#include "../FloaterUtil/include/FloaterMacro.h"


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	//EditorApplication a(argc, argv);

	MainWindow mainWindow;
	mainWindow.show();

	return a.exec();
}
