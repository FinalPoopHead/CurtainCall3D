#include "EditorApplication.h"
#include "GameView.h"

EditorApplication::EditorApplication(int& argc, char** argv)
	: QApplication(argc, argv)
{

}

EditorApplication::~EditorApplication()
{

}

bool EditorApplication::notify(QObject* receiver, QEvent* event)
{
	if (auto gameView = qobject_cast<GameView*>(receiver))
	{
		if (!gameView->gameUpdate())
		{
			return false;
		}
	}

	return QApplication::notify(receiver, event);
}
