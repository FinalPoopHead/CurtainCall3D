#include "TitleBar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QWindow>
#include <QStyleOption>
#include <QPainter>


namespace
{
	class CustomButton : public QPushButton
	{
	public:
		CustomButton(QWidget* parent = nullptr)
			: QPushButton(parent)
		{
			setFixedSize(20, 20);
		}

	protected:
		virtual void drawIcon(QPainter& p) = 0;

		void paintEvent(QPaintEvent* event) override
		{
			QStyleOption opt;
			opt.initFrom(this);
			QPainter p(this);
			style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

			// 버튼의 배경색 설정
			p.fillRect(rect(), QColor(0xf0, 0xf0, 0xf0));

			// 호버 상태일 때 배경색 변경
			if (underMouse()) {
				p.fillRect(rect(), QColor(0x65, 0x65, 0x65));
			}

			p.setRenderHint(QPainter::Antialiasing);
			drawIcon(p);
		}
	};

	class MinimizeButton : public CustomButton
	{
	public:
		MinimizeButton(QWidget* parent = nullptr) : CustomButton(parent) {}
	protected:
		void drawIcon(QPainter& p) override {
			p.setPen(QPen(Qt::black, 0.8f)); 
			p.drawLine(5, 13, 15, 13);
		}
	};

	class MaximizeButton : public CustomButton
	{
	public:
		MaximizeButton(QWidget* parent = nullptr) : CustomButton(parent) {}
	protected:
		void drawIcon(QPainter& p) override {
			p.setPen(QPen(Qt::black, 0.8f));
			p.drawRect(5, 5, 10, 10);
		}
	};

	class CloseButton : public CustomButton
	{
	public:
		CloseButton(QWidget* parent = nullptr) : CustomButton(parent) {}
	protected:
		void drawIcon(QPainter& p) override {
			p.setPen(QPen(Qt::black, 1.0f));
			p.drawLine(5, 5, 15, 15);
			p.drawLine(5, 15, 15, 5);
		}
	};
}


TitleBar::TitleBar(std::string titleName, QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{
	setFixedHeight(20);
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(5, 0, 5, 0);

	QLabel* titleLabel = new QLabel(titleName.c_str(), this);
	_minimizeButton = new MinimizeButton(this);
	_maximizeButton = new MaximizeButton(this);
	_closeButton = new CloseButton(this);

	QWidget* buttonContainer = new QWidget(this);
	QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	buttonLayout->addWidget(_minimizeButton);
	buttonLayout->addWidget(_maximizeButton);
	buttonLayout->addWidget(_closeButton);

	layout->addWidget(titleLabel);
	layout->addStretch();
	layout->addWidget(buttonContainer);

	//setStyleSheet(R"(
	//	TitleBar 
	//	{
	//		color: #3C3C3C;
	//	}

	//	QLabel 
	//	{
	//		color: #1E1E1E;
	//	}
	//)");

	connect(_minimizeButton, &QPushButton::clicked, this, &TitleBar::minimizeClicked);
	connect(_maximizeButton, &QPushButton::clicked, this, &TitleBar::maximizeClicked);
	connect(_closeButton, &QPushButton::clicked, this, &TitleBar::closeClicked);
}

TitleBar::~TitleBar()
{

}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	if (event->button() == Qt::LeftButton)
	{
		_dragPosition = event->globalPosition().toPoint() - parentWidget()->frameGeometry().topLeft();
		event->accept();
	}
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
	if (event->buttons() & Qt::LeftButton)
	{
		parentWidget()->move(event->globalPosition().toPoint() - _dragPosition);
		event->accept();
	}
}
