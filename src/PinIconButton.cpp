#include <QPushButton>
#include <QProcess>
#include "DesktopFile.h"
#include "PinIconButton.h"

int PinIconButton::NumberOfIcons = 0;
std::vector<PinIconButton *> *PinIconButton::Icons = new std::vector<PinIconButton *>();

PinIconButton *PinIconButton::NewIcon(std::string DesktopFileExactName, std::string Action, size_t *ID) {
	auto Button = new PinIconButton();
	Button->Btn = new QPushButton();
	Button->Btn->setFixedSize(QSize(46, 46));
	//Button->setGeometry(2 + 50*NumberOfIcons, 2, 48 + 50*NumberOfIcons, 48);
	std::string IconPath = DesktopFile::GetIcon(DesktopFile::GetDekstopFileIconName(DesktopFileExactName), 46);
	Button->Btn->setIcon(QIcon(QString::fromStdString(IconPath)));
	Button->Btn->setIconSize(Button->Btn->size());
	QObject::connect(Button->Btn, &QPushButton::clicked, [Action]() {
		QProcess Process;
		Process.setProgram("sh");
		Process.setArguments({"-c", QString::fromStdString(Action)});
		Process.startDetached();
	});
	if (ID != nullptr) *ID = Icons->size();
	Icons->push_back(Button);
	return Button;
}

QPushButton *PinIconButton::GetButton() {
	return Btn;
}
