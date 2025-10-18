#ifndef WINDOWBUTTON_H
#define WINDOWBUTTON_H

#include <unordered_map>
#include <string>
#include <vector>
#include <QLayout>
#include "PinIconButton.h"

class OpenWindowButtons {
	private:
		std::unordered_map<std::string, std::vector<std::string> *> *Apps;
		QLayout *UsedLayout;
		std::vector<PinIconButton *> *Icons;
	public:
		void Reset();
		void RegisterNew(std::string App, std::string Action);
		void Update();
		OpenWindowButtons(QLayout *Layout);
};

#endif
