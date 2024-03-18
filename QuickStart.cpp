#include "QuickStart.h"
#include "qfile.h"
#include "qtextstream.h"

bool QuickStart::load(const std::string &path)
{
	QFile file(path.c_str());
	if (!file.open(QIODevice::ReadOnly))
		return false;
	QTextStream in(&file);
	in.setAutoDetectUnicode(true);
	QString all = in.readAll();

	file.close();

	QJsonParseError err;
	doc = QJsonDocument::fromJson(all.toUtf8(), &err);
	if (err.error != QJsonParseError::NoError and !doc.isNull())
		return false;

	QJsonObject root = doc.object();
	QJsonValue nameValue = root.value("name");
	QJsonValue pathValue = root.value("path");
}

bool QuickStart::save(const std::string &path)
{
	return true;
}

std::vector<std::pair<std::string, std::string>> &QuickStart::getFiles()
{
	return;
}

void QuickStart::addFile(const std::string &name, const std::string &path)
{
	files.push_back(std::make_pair(name, path));
}