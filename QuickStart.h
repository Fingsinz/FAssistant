#pragma once

#include "qjsonobject.h"
#include "qjsondocument.h"
#include "qjsonarray.h"
#include "qjsonvalue.h"

class QuickStart
{
	QJsonDocument doc;
	std::vector<std::pair<std::string, std::string>> files;

public:

	bool load(const std::string &path);

	bool save(const std::string &path);

	std::vector<std::pair<std::string, std::string>> &getFiles();

	void addFile(const std::string &name, const std::string &path);
};
