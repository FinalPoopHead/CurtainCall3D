#pragma once
#include <QWidget>

class InspectorView : public QWidget
{
	Q_OBJECT
public:
	InspectorView(QWidget* parent = nullptr) {}
	~InspectorView() {}

	void resizeEvent(QResizeEvent* event) override {}
};
