#pragma once
#include <string>
#include <vector>
#include <windows.h>  //include all the basics
class SearchFiles
{
private:
	std::vector <std::wstring> * files;
public:
	SearchFiles();
	std::vector <std::wstring> * run(std::string name);
	~SearchFiles();
};

