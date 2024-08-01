#include <QWidget>


class QPushButton;

class TitleBar : public QWidget
{
	Q_OBJECT
public:
	TitleBar(std::string titleName, QWidget* parent = nullptr);
	~TitleBar();

signals:
	void minimizeClicked();
	void maximizeClicked();
	void closeClicked();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private slots:
	void updateMaximizeButton(Qt::WindowState newState);

private:
	QPoint _dragPosition;

	QPushButton* _minimizeButton;
	QPushButton* _maximizeButton;
	QPushButton* _closeButton;
};
