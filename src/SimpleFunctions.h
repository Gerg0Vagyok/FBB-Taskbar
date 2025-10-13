#ifndef SIMPLEFUNCTIONS_H
#define SIMPLEFUNCTIONS_H

#include <string>
#include <vector>
#include <sys/stat.h>
#include <unicode/locid.h>

extern std::vector<std::string> *Split(std::string Value, char Separator);
extern std::string FormatPath(std::string Path, bool UseSlash);
extern std::vector<std::string> *FindInFolder(std::string Path, std::string Name, bool CaseSensitive, bool ReturnFullPath);
extern int FileExsists(std::string Path);

#endif
