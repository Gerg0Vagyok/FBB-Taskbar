#ifndef DESKTOPFILE_H
#define DESKTOPFILE_H

#include <sys/stat.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <LayerShellQt/Window>
#include <string>
#include <vector>

class DesktopFile {
	private:
		static std::string DefaultIconName;
		static std::string IconPath;
		static std::string DesktopPath;
		static std::vector<std::string> *IconPathSplit;
		static std::vector<std::string> *DesktopPathSplit;

		std::string DesktopFileString;
		std::string DesktopFileIcon;
	public:
		static std::string GetIconPath();
		static void SetIconPath(std::string Value);
		static std::string GetDesktopPath();
		static void SetDesktopPath(std::string Value);

		DesktopFile(std::string DesktopFileName);

		static std::string GetDekstopFileIconName(std::string Name);
		static std::string GetIcon(std::string Name, int DesiredSize);
		std::string GetIcon(int DesiredSize);
};

#endif
