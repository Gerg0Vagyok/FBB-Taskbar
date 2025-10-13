#include <QPushButton>
#include <QProcess>
#include "DesktopFile.h"
#include "PinIconButton.h"

int PinIconButton::NumberOfIcons = 0;

PinIconButton::PinIconButton(std::string DesktopFileExactName, std::string Action) {
	BUTTON = new QPushButton();
	BUTTON->setFixedSize(QSize(46, 46));
	//BUTTON->setGeometry(2 + 50*NumberOfIcons, 2, 48 + 50*NumberOfIcons, 48);
	std::string IconPath = DesktopFile::GetIcon(DesktopFile::GetDekstopFileIconName(DesktopFileExactName), 46);
	BUTTON->setIcon(QIcon(QString::fromStdString(IconPath)));
	BUTTON->setIconSize(BUTTON->size());
	QObject::connect(BUTTON, &QPushButton::clicked, [Action]() {
		QProcess Process;
		Process.setProgram("sh");
		Process.setArguments({"-c", QString::fromStdString(Action)});
		Process.startDetached();
	});
	NumberOfIcons++;
}
QPushButton *PinIconButton::GetButton() {
	return BUTTON;
}
