#ifndef PINICONBUTTON_H
#define PINICONBUTTON_H

#include <QPushButton>
#include <QProcess>

class PinIconButton {
	private:
		static int NumberOfIcons;
		QPushButton *BUTTON;
	public:
		PinIconButton(std::string DesktopFileExactName, std::string Action);
		QPushButton *GetButton();
};

#endif
