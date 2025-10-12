#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QObject>
#include <QProcess>
#include <LayerShellQt/Window>

class DesktopFile {
	private:
		inline static std::string DefaultIcon = "fbd-missing";
	public:
		static std::string GetIcon() {
			return "";
		}
};

class PinIconButton {
	private:
		inline static int NumberOfIcons = 0;
		QPushButton *BUTTON;
	public:
		PinIconButton(QWidget *Window, QString DesktopFile) {
			BUTTON = new QPushButton("", Window);
			BUTTON->setGeometry(2 + 50*NumberOfIcons, 2, 48 + 50*NumberOfIcons, 48);
			BUTTON->setIcon(QIcon(Icon));
			BUTTON->setIconSize(BUTTON->size());
			QObject::connect(BUTTON, &QPushButton::clicked, [Action]() {
				QProcess::startDetached("sh", QStringList() << "-c" << Action);
			});
			NumberOfIcons++;
		}

		QPushButton *GetButton() {
			return BUTTON;
		}
};

class IconTrayButton { // Fix this
	private:
		inline static int NumberOfIcons = 0;
		QPushButton *BUTTON;
	public:
		IconTrayButton(QWidget *Window, QString Icon, QString Action) {
			BUTTON = new QPushButton("", Window);
			BUTTON->setGeometry(15 + 20*NumberOfIcons, 2,  + 20*NumberOfIcons, 48);
			BUTTON->setIcon(QIcon(Icon));
			BUTTON->setIconSize(BUTTON->size());
			QObject::connect(BUTTON, &QPushButton::clicked, [Action]() {
				QProcess::startDetached("sh", QStringList() << "-c" << Action);
			});
			NumberOfIcons++;
		}

		QPushButton *GetButton() {
			return BUTTON;
		}
};

int main(int argc, char **argv) {
	int ScreenID = 0;
	QApplication app (argc, argv);

	auto Screen = QGuiApplication::screens().at(ScreenID);

	int Width  = Screen->geometry().width();

	QWidget Window;
	Window.setWindowFlags(Qt::FramelessWindowHint);
	Window.setMaximumSize(QSize(Width, 50));
	Window.setMinimumSize(QSize(Width, 50));

	Window.winId();
	auto *LayerWindow = LayerShellQt::Window::get(Window.windowHandle());
	LayerWindow->setLayer(LayerShellQt::Window::LayerTop);
	LayerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);
	LayerWindow->setExclusiveZone(50);
	LayerWindow->setAnchors(LayerShellQt::Window::AnchorBottom);

	PinIconButton *NewBtn1 = new PinIconButton(&Window, "kitty"); // Test button, this isnt really a good use to anyone but me.

	Window.show();
	return app.exec();
}

