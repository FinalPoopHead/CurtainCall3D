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

	//FILE* console = freopen("CONOUT$", "w", stdout);
	//ASSERT(console, "Failed to open console window");
	//std::cout.clear();

	while(true)
	{
		mainWindow.Update();
		a.processEvents();
	}

	//fclose(console);

	return a.exec();
}
