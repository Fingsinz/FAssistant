#pragma once

#include "qfile.h"
#include "qtextstream.h"

class QuickStart
{
	std::vector<std::pair<std::string, std::string>> files;

public:

	bool load(const std::string &path);

	bool save(const std::string &path);

	std::vector<std::pair<std::string, std::string>> &getFiles();

	void addFile(const std::string &name, const std::string &path);

	void modifyFile(const std::string &oldName, const std::string &newName = "",
		const std::string &newPath = "");

	void removeFile(const std::string &name);
};
