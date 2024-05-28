#include "pch.h"
#include "file_dialog.hpp"
#include <core/application.hpp>
#include <utils/assert.hpp>

#include <tinyfiledialogs.h>
#include <vector>
#include <sstream>

std::string hyp::FileDialog::openFile(const std::string& filter, const std::string& filterDescription, bool multiSelect) {
	std::vector<const char*> substrings;
	std::istringstream iss(filter);
	std::string token;
	while (std::getline(iss, token, '\0'))
	{
		substrings.push_back(token.c_str());
	}
	auto file = tinyfd_openFileDialog("Select file to open", "", substrings.size(),
	    substrings.data(), filterDescription.empty() ? NULL : filterDescription.c_str(), (int)multiSelect);

	if (!file) return std::string();

	return file;
}

std::string hyp::FileDialog::saveFile(const std::string& filter, const std::string& description) {
	std::vector<const char*> substrings;
	std::istringstream iss(filter);
	std::string token;
	while (std::getline(iss, token, '\0'))
	{
		substrings.push_back(token.c_str());
	}
	auto file = tinyfd_saveFileDialog("Select file to save", "", substrings.size(),
	    substrings.data(), description.empty() ? NULL : description.c_str());

	if (!file) return std::string();

	return file;
}