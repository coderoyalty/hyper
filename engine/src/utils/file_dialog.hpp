#pragma once
#ifndef HYP_FILE_DIALOG_HPP
	#define HYP_FILE_DIALOG_HPP
	#include <string>
namespace hyp {

	// The FileDialog class provides methods for opening and saving files through a dialog window.
	// @param filter: a string containing filter patterns separated by '\0'. e.g., "*.txt\0*.hypscene\0".
	// @param description: an optional description to be displayed in the file dialog window.
	// @return a string representing the selected file path. If multi-selection is true, returns a semicolon-separated list of selected file paths.
	class FileDialog {
	public:
		/**
		* @brief Opens a file dialog window for selecting files.
		*
		* @param multiSelect If true, allows selecting multiple files; otherwise, only a single file can be selected (default is false).
		*/
		static std::string openFile(const std::string& filter, const std::string& description = "", bool multiSelect = false);

		/**
		* @brief Opens a file dialog window for saving a file.
		*
		* @return A string representing the selected file path for saving.
		*/
		static std::string saveFile(const std::string& filter, const std::string& description = "");
	};

}

#endif