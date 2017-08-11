#include "SearchFiles.h"
#include <experimental\filesystem>
#include <exception>

SearchFiles::SearchFiles()
{
	files = new std::vector <std::wstring>;
}

std::vector<std::wstring>* SearchFiles::run(std::string name)
{
	
	//files->clear();
	for (auto & p : std::experimental::filesystem::recursive_directory_iterator(name)) {
		try {
			if (p.path().wstring().size() >= 253){
				
				files->push_back(p.path().wstring());
			}
		// files->push_back("a");
		}

		catch (std::exception& e) {

			MessageBox(
				NULL,
				(LPCTSTR)L"Something goes wrong... :(",
				(LPCWSTR)L"Check the length",
				MB_OK
			);
			exit(-1);
		}
		//std::experimental::filesystem::file_status status(p.path);
		//if (std::experimental::filesystem::is_directory(p.path())) {
		//	this->run(p.path().string());
		//}
	}
	return files;
}


SearchFiles::~SearchFiles()
{
	files->clear();
	delete files;
}
