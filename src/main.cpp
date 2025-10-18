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
#include <iostream>

#include "PinIconButton.h"
#include "OpenWindowButtons.h"
#include "SimpleFunctions.h"

int main(int argc, char **argv) {
	if (PrevligeCheck() == true) {
		std::cout << "NOT SAFE!\nRun app as a regular user to continue.\n";
		return 1;
	}

	int ScreenID = 0;
	QApplication app (argc, argv);

	auto Screen = QGuiApplication::screens().at(ScreenID);

	int Width  = Screen->geometry().width();

	QWidget Window;
	Window.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus | Qt::Tool);
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

	QHBoxLayout *IconsLayout = new QHBoxLayout();
	IconsLayout->setSpacing(2);
	IconsLayout->setContentsMargins(0, 0, 0, 0);

	PinIconButton *TestIcon = PinIconButton::NewIcon("Crowbar", "kitty", nullptr);
	IconsLayout->addWidget(TestIcon->GetButton(), 0);

	PinIconButton *TestIcon2 = PinIconButton::NewIcon("asdasd", "floorp", nullptr);
	IconsLayout->addWidget(TestIcon2->GetButton(), 0);
	
	IconsLayout->addWidget(HorizontalSeparator(), 0);

	QProcess *GetActiveWindowsProcess = new QProcess();
	GetActiveWindowsProcess->setProgram("/home/Gerg0Vagyok/projects/SwayGetWindowsAndIcons/bin/main");
	/* 
	 * Since i currently i dont know of a way to make this compatible across all window managers and desktop envirenments,
	 * this uses an executable i wrote in which doesnt really exsist for anyone else. and i dont plan on implementing it since it would be different per wm/de.
	 * Will have a config option for this. Hopefulyy nobody else tries this.
	 * */
	GetActiveWindowsProcess->setArguments({});
	GetActiveWindowsProcess->start();

	auto Testicons = new OpenWindowButtons(IconsLayout);

	QObject::connect(GetActiveWindowsProcess, &QProcess::readyReadStandardOutput, [&GetActiveWindowsProcess, Testicons]() {
		QByteArray data = GetActiveWindowsProcess->readAllStandardOutput();
		Testicons->Reset();

		//qDebug() << "Received bytes:" << data.size();
		QList List = data.split('\n');
		for (QByteArray El : List) {
			QList Window = El.split('\0');
			if (Window.size() == 3) {
				Testicons->RegisterNew(Window[1].toStdString(), Window[2].toStdString());
				/*qDebug() << "Type: " << Window[0];
				qDebug() << "Icon: " << Window[1];
				qDebug() << "Action: " << Window[2];
				*/
			}
		}
		Testicons->Update();
	});

	std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm *tm = std::localtime(&t);
	std::ostringstream TimeLabelString;
	TimeLabelString << std::put_time(tm, "%H:%M:%S");
	QWidget *TimeLabel = new QLabel(QString::fromStdString(TimeLabelString.str()));
	TimeLabelString.str("");
	TimeLabelString.clear();
	TimeLabelString << std::put_time(tm, "%Y %m %d");
	QWidget *DateLabel = new QLabel(QString::fromStdString(TimeLabelString.str()));
	
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

