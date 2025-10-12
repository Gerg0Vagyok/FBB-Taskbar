#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QObject>
#include <QProcess>
#include <sstream>
#include <filesystem>
#include <sys/stat.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <LayerShellQt/Window>

std::vector<std::string> *Split(std::string Value, char Separator) {
	std::vector<std::string> *Out = new std::vector<std::string>();
	std::string CrntString = "";
	for (size_t i = 0; i < Value.size(); i++) {
		char CrntChar = Value.c_str()[i];
		if (CrntChar == Separator) {
			Out->push_back(CrntString);
			CrntString.clear();
		} else {
			CrntString += CrntChar;
		}
	}
	return Out;
}

std::string FormatPath(std::string Path, bool UseSlash) {
	if (UseSlash) {
		if (Path.back() == '/') {
			return Path;
		} else {
			return Path + '/';
		}
	} else {
		if (Path.back() != '/') {
			return Path;
		} else {
			return Path.substr(0, Path.size() - 1);
		}
	}
}

std::vector<std::string> *FindInFolder(std::string Path, std::string Name, bool CaseSensitive, bool ReturnFullPath) {
	std::vector<std::string> *ReturnVar = new std::vector<std::string>();
	std::filesystem::path PathVar{Path};
	auto FormattedPath = FormatPath(Path, true);

	if (CaseSensitive) {
		for (auto const& Entry : std::filesystem::recursive_directory_iterator{PathVar}) {
			std::string PathName = Entry.path().string().substr(FormattedPath.size(), Entry.path().string().size());
			if (PathName.find(Name) != std::string::npos) {
				if (ReturnFullPath) {
					ReturnVar->push_back(Entry.path());
				} else {
					ReturnVar->push_back(PathName);
				}
			}
		}
	} else {
		for (auto const& Entry : std::filesystem::recursive_directory_iterator{PathVar}) {
			std::string PathName;
			std::string NewName;
			(new icu::UnicodeString(Entry.path().string().c_str(), "UTF-8"))->toLower("en_US").toUTF8String(PathName);
			(new icu::UnicodeString(Name.c_str(), "UTF-8"))->toLower("en_US").toUTF8String(NewName);
			PathName = PathName.substr(FormattedPath.size(), Entry.path().string().size());
			if (PathName.find(NewName) != std::string::npos) {
				if (ReturnFullPath) {
					ReturnVar->push_back(Entry.path());
				} else {
					ReturnVar->push_back(Entry.path().string().substr(FormattedPath.size(), Entry.path().string().size()));
				}
			}
		}
	}
	return ReturnVar;
} // the function i have made in c but implemented in cpp using functions avialable here.

int FileExsists(std::string Path) {
	struct stat ThrowawayVar;
	return stat(Path.c_str(), &ThrowawayVar);
}

class DesktopFile {
	private:
		inline static std::string DefaultIconName = "/usr/share/icons/hicolor/128x128/apps/crowbar.png"; //"fbd-missing";
			// Since i dont actually have anything yet to have i will just use one i have.
		inline static std::string IconPath = "/usr/share/icons/hicolor/";
		inline static std::vector<std::string> *IconPathSplit = Split(IconPath, '/');
	public:
		inline static std::string DesktopPath = "/usr/share/applications/";

		static std::string GetIconPath() {
			return IconPath;
		}
		static void SetIconPath(std::string Value) {
			IconPath = Value;
			IconPathSplit = Split(IconPath, '/');
		}

		static std::string GetIcon(std::string Name, int DesiredSize) {
			std::vector<int> *FoundSizes = new std::vector<int>();
			std::map<int, std::string> Map;

			auto FoundFiles = FindInFolder(IconPath, Name, false, true);
			if (FoundFiles->size() > 0) {
				for (const std::string &el : *FoundFiles) {
					auto el_vec = Split(el, '/');
					if (IconPathSplit->size() < el_vec->size()) {
						std::string V = (*el_vec)[IconPathSplit->size()];
						int val;
						std::istringstream(V.substr(0, (V.size()-1)/2)) >> val;
						FoundSizes->push_back(val);
						Map[val] = el;
					}
				}

				if (std::count(FoundSizes->begin(), FoundSizes->end(), DesiredSize) > 0) return Map[DesiredSize];

				std::sort(FoundSizes->begin(), FoundSizes->end());
				if ((*FoundSizes)[FoundSizes->size()] < DesiredSize) return Map[(*FoundSizes)[FoundSizes->size()]];
				if ((*FoundSizes)[0] > DesiredSize) return Map[(*FoundSizes)[0]];

				for (int &el : *FoundSizes) {
					if (el > DesiredSize) return Map[el];
				}
			}
			return DefaultIconName;
		}
};
/*
class PinIconButton {
	private:
		inline static int NumberOfIcons = 0;
		QPushButton *BUTTON;
	public:
		PinIconButton(QWidget *Window, std::string DesktopFile) {
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
};*/

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

	//auto O = FindInFolder("/usr/share/icons/hicolor", "Crow", false, false);
	//for (const std::string &el : *O) {
	//	std::cout << el << '\n';
	//}
	DesktopFile::GetIcon("crow", 46);

	//PinIconButton *NewBtn1 = new PinIconButton(&Window, "kitty"); // Test button, this isnt really a good use to anyone but me.

	Window.show();
	return app.exec();
}

