#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QObject>
#include <QProcess>
#include <QLabel>
#include <QTimer>
#include <QHBoxLayout>
#include <sstream>
#include <sys/stat.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <LayerShellQt/Window>
#include <chrono>
#include <iomanip>

#include "PinIconButton.h"

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

	//auto O = FindInFolder("/usr/share/icons/hicolor", "Crow", false, false);
	//for (const std::string &el : *O) {
	//	std::cout << el << '\n';
	//}
	//DesktopFile::GetIcon("crow", 46);

	//PinIconButton *NewBtn1 = new PinIconButton(&Window, "kitty"); // Test button, this isnt really a good use to anyone but me.
	
	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm *tm = std::localtime(&t);
	std::ostringstream TimeLabelString;
	TimeLabelString << std::put_time(tm, "%H:%M:%S");
	QWidget *TimeLabel = new QLabel(QString::fromStdString(TimeLabelString.str()));
	TimeLabelString.str("");
	TimeLabelString.clear();
	TimeLabelString << std::put_time(tm, "%Y %m %d");
	QWidget *DateLabel = new QLabel(QString::fromStdString(TimeLabelString.str()));

	QHBoxLayout *IconsLayout = new QHBoxLayout();
	IconsLayout->setSpacing(0);
	IconsLayout->setContentsMargins(0, 0, 0, 0);

	PinIconButton *TestIcon = new PinIconButton("Crowbar", "kitty");
	IconsLayout->addWidget(TestIcon->GetButton(), 0);

	QVBoxLayout *TimeDateLayout = new QVBoxLayout;
	TimeDateLayout->setContentsMargins(0, 6, 6, 6);
	TimeDateLayout->addWidget(DateLabel, 1);
	TimeDateLayout->addWidget(TimeLabel, 1);
	
	TimeDateLayout->setAlignment(TimeLabel, Qt::AlignmentFlag::AlignHCenter);
	TimeDateLayout->setAlignment(DateLabel, Qt::AlignmentFlag::AlignHCenter);

	QHBoxLayout *MainLayout = new QHBoxLayout;
	MainLayout->setContentsMargins(2, 2, 6, 2);

	MainLayout->addLayout(IconsLayout, 0); // THIS IS ON THE LEFT
	MainLayout->addLayout(TimeDateLayout, 0);

	//MainLayout->setAlignment(IconsLayout, Qt::AlignmentFlag::AlignLeft);
	MainLayout->setAlignment(TimeDateLayout, Qt::AlignmentFlag::AlignRight);

	QObject *DateAndTimeTimerObject = new QObject();
	QTimer *DateAndTimeTimer = new QTimer(DateAndTimeTimerObject);
	QObject::connect(DateAndTimeTimer, &QTimer::timeout, DateAndTimeTimerObject, [TimeLabel, DateLabel]() {
		std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::tm *tm = std::localtime(&t);
		std::ostringstream TimeLabelString;
		TimeLabelString << std::put_time(tm, "%H:%M:%S");
		((QLabel*)TimeLabel)->setText(QString::fromStdString(TimeLabelString.str()));
		TimeLabel->update();
		TimeLabelString.str("");
		TimeLabelString.clear();
		TimeLabelString << std::put_time(tm, "%Y %m %d");
		((QLabel*)DateLabel)->setText(QString::fromStdString(TimeLabelString.str()));
		DateLabel->update();
	});
	DateAndTimeTimer->start(1000);

	Window.setLayout(MainLayout);

	Window.show();
	return app.exec();
}

