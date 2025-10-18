#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <LayerShellQt/Window>
#include <string>
#include <vector>
#include <iostream>

#include "SimpleFunctions.h"
#include "DesktopFile.h"

std::string DesktopFile::DefaultIconName = "/usr/share/icons/hicolor/128x128/apps/fbb-taskbar-missing.png";
std::string DesktopFile::IconPath = "/usr/share/icons/hicolor/";
std::string DesktopFile::DesktopPath = "/usr/share/applications/";
std::vector<std::string> *DesktopFile::IconPathSplit = Split(IconPath, '/');
std::vector<std::string> *DesktopFile::DesktopPathSplit = Split(DesktopPath, '/');

std::string DesktopFile::GetIconPath() {
	return IconPath;
}
void DesktopFile::SetIconPath(std::string Value) {
	IconPath = Value;
	IconPathSplit = Split(IconPath, '/');
}
std::string DesktopFile::GetDesktopPath() {
	return DesktopPath;
}
void DesktopFile::SetDesktopPath(std::string Value) {
	DesktopPath = Value;
	DesktopPathSplit = Split(DesktopPath, '/');
}

DesktopFile::DesktopFile(std::string DesktopFileName) {
	this->DesktopFileString = DesktopFileName;
	this->DesktopFileIcon = DesktopFile::GetDekstopFileIconName(DesktopFileName);
}

std::string DesktopFile::GetDekstopFileIconName(std::string Name) {
	std::ostringstream File_oss;
	File_oss << FormatPath(DesktopPath, true) << Name << ".desktop";
	std::ifstream infile(File_oss.str());

	std::string Line;
	while (std::getline(infile, Line)) {
		std::cout << Line << '\n';
		if (Line.size() > 5 && Line.substr(0, 5).find("Icon=")) {
			return Line.substr(5, Line.size()-1);
		}
	}
	return "";
}

std::string DesktopFile::GetIcon(std::string Name, int DesiredSize) {
	if (Name.size() > 0) {
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
	}
	return DefaultIconName;
}

std::string DesktopFile::GetIcon(int DesiredSize) {
	return DesktopFile::GetIcon(this->DesktopFileIcon, DesiredSize);
}
