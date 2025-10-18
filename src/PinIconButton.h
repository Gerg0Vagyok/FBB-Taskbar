#ifndef PINICONBUTTON_H
#define PINICONBUTTON_H

#include <QPushButton>
#include <QProcess>

class PinIconButton {
	private:
		static int NumberOfIcons;
		static std::vector<PinIconButton *> *Icons;
		QPushButton *Btn;
	public:
		//PinIconButton(std::string DesktopFileExactName, std::string Action);
		QPushButton *GetButton();
		static void Delete(PinIconButton *Instance);
		static PinIconButton *NewIcon(std::string DesktopFileExactName, std::string Action, size_t *ID);
};

#endif
