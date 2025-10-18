#include <string>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <unicode/locid.h>
#include <QFrame>
#include <unistd.h>

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

QFrame *HorizontalSeparator() {
	auto Sep = new QFrame();
	Sep->setFrameShape(QFrame::VLine);
	Sep->setFrameShadow(QFrame::Plain);
	Sep->setLineWidth(1);
	return Sep;
}

QFrame *VerticalSeparator() {
	auto Sep = new QFrame();
	Sep->setFrameShape(QFrame::HLine);
	Sep->setFrameShadow(QFrame::Plain);
	Sep->setLineWidth(1);
	return Sep;
}

bool PrevligeCheck() {
	return geteuid() == 0;
}
