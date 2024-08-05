#include <QtWidgets/QApplication>

class EditorApplication : public QApplication
{
public:
	EditorApplication(int& argc, char** argv);
	~EditorApplication();

	bool notify(QObject* receiver, QEvent* event) override;
};
