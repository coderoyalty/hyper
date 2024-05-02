#pragma once
#ifndef HYP_FILE_DIALOG_HPP
	#define HYP_FILE_DIALOG_HPP
	#include <string>
namespace hyp {
	class FileDialog {
	public:
		static std::string openFile(const char* filter);

		static std::string saveFile(const char* filter);
	};
}

#endif