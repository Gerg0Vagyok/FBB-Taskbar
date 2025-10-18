#include "OpenWindowButtons.h"

void OpenWindowButtons::Reset() {
	Apps->clear();
	for (auto el : *Icons) {
		PinIconButton::Delete(el);
	}
	Icons->clear();
}

void OpenWindowButtons::RegisterNew(std::string App, std::string Action) {
	if (Apps->count(App) == 0) {
		Apps->insert(std::make_pair(App, new std::vector<std::string>()));
	}
	Apps->at(App)->push_back(Action);
}

void OpenWindowButtons::Update() {
	for (auto& [Key, Value]: *Apps) {
		for (auto el : *Value) {
			auto NewIcon = PinIconButton::NewIcon(Key, el, nullptr);
			Icons->push_back(NewIcon);
			UsedLayout->addWidget(NewIcon->GetButton());
		}
	}
	UsedLayout->update();
}

OpenWindowButtons::OpenWindowButtons(QLayout *Layout) {
	Apps = new std::unordered_map<std::string, std::vector<std::string> *>();
	UsedLayout = Layout;
	Icons = new std::vector<PinIconButton *>();
}


