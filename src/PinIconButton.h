#ifndef PINICONBUTTON
#define PINICONBUTTON

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
