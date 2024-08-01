#include "TitleBar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QWindow>

TitleBar::TitleBar(std::string titleName, QWidget* parent /*= nullptr*/)
	: QWidget(parent)
{
	setFixedHeight(20);
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(5, 0, 5, 0);

	QLabel* titleLabel = new QLabel(titleName.c_str(), this);
	_minimizeButton = new QPushButton("－", this);
	_maximizeButton = new QPushButton("□", this);
	_closeButton = new QPushButton("×", this);
	_minimizeButton->setFixedSize(20, 20);
	_maximizeButton->setFixedSize(20, 20);
	_closeButton->setFixedSize(20, 20);

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

	setStyleSheet(R"(
		TitleBar 
		{
			color: #3C3C3C;
		}

		QLabel 
		{
			color: #1E1E1E;
		}

		QPushButton 
		{
			border: none;
			background-color: transparent;
			color: #1E1E1E;
		}

		QPushButton:hover 
		{
			background-color: #656565;
        }

		#closeButton:hover
		{
			font-size: 1px;
		}
	)");

	_minimizeButton->setStyleSheet(R"(QPushButton { border: none; })");
	_closeButton->setStyleSheet(R"(QPushButton { border: none; })");

	connect(_minimizeButton, &QPushButton::clicked, this, &TitleBar::minimizeClicked);
	connect(_maximizeButton, &QPushButton::clicked, this, &TitleBar::maximizeClicked);
	connect(_closeButton, &QPushButton::clicked, this, &TitleBar::closeClicked);
	//connect(parentWidget(), &QWindow::windowStateChanged, this, &TitleBar::updateMaximizeButton);
}

TitleBar::~TitleBar()
{

}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
	if(event->button() == Qt::LeftButton)
	{
		_dragPosition = event->globalPosition().toPoint() - parentWidget()->frameGeometry().topLeft();
		event->accept();
	}
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	QWidget::mouseMoveEvent(event);
	if(event->buttons() & Qt::LeftButton)
	{
		parentWidget()->move(event->globalPosition().toPoint() - _dragPosition);
		event->accept();
	}
}

void TitleBar::updateMaximizeButton(Qt::WindowState newState)
{
	bool isMaximized = newState & Qt::WindowState::WindowMaximized;
	_maximizeButton->setText(isMaximized ? "❐" : "□");
}
