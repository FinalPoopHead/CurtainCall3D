#pragma once
#include <QWidget>

class SceneView : public QWidget
{
	Q_OBJECT
public:
	SceneView(QWidget* parent = nullptr) {}
	~SceneView() {}

	void resizeEvent(QResizeEvent* event) override {}
	void closeEvent(QCloseEvent* event) override {}

	bool isPlaying() {}

private:
};
