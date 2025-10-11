#include <QGuiApplication>
#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QWindow>

int main(int argc, char **argv) {
	QApplication app (argc, argv);

	QWidget Window;
	Window.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
						  Qt::Tool | Qt::WindowDoesNotAcceptFocus);

	QWindow *QWindowVar = Window.windowHandle();
	if (!QWindowVar) {
		Window.show();
		Window.hide();
		QWindowVar = Window.windowHandle();
	}

	if (QWindowVar) {
		QPlatformNativeInterface *Native = QGuiApplication::platformNativeInterface();
		if (Native) {
			Native->setWindowProperty();
		}
	}

	QPushButton *button = new QPushButton("Overlay Button", &Window);
	Window.show();

	return app.exec();
}

