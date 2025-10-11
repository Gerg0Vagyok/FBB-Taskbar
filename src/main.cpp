#include <QApplication>
#include <QPushButton>
#include <QWidget>

int main(int argc, char **argv) {
	QApplication app (argc, argv);

	QWidget Window;
	Window.setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
	Window.resize(300, 100);

	QPushButton *button = new QPushButton("Overlay Button", &Window);

	Window.show();

	return app.exec();
}

