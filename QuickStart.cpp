#include "QuickStart.h"
#include "qstringconverter.h"

void QuickStart::setRecordPath(std::string const &path)
{
	recordPath = path;
}

std::string QuickStart::getRecordPath()
{
	return recordPath;
}

bool QuickStart::load(const std::string &path)
{
	files.clear();
	std::vector<std::pair<std::string, std::string>>().swap(files);

	QFile file(path.c_str());
	QTextStream in(&file);
	in.setEncoding(QStringConverter::Encoding::Utf8);

	if (!file.open(QIODevice::ReadOnly))
		return false;

	while (!in.atEnd())
	{
		QString line = in.readLine(); // name:path
		int index = line.indexOf(',');
		files.push_back(std::make_pair(line.left(index).toStdString(), line.mid(index + 1).toStdString()));
	}

	return true;
}

bool QuickStart::save(const std::string &path)
{
	QFile file(path.c_str());
	QTextStream out(&file);

	if (!file.open(QIODevice::WriteOnly))
		return false;

	for (auto &file : files)
		out << QString::fromStdString(file.first) << ',' << QString::fromStdString(file.second) << '\n';

	return true;
}

std::vector<std::pair<std::string, std::string>> &QuickStart::getFiles()
{
	return files;
}

void QuickStart::addFile(const std::string &name, const std::string &path)
{
	files.push_back(std::make_pair(name, path));
}

void QuickStart::modifyFile(const std::string &oldName, const std::string &newName,
	const std::string &newPath)
{
	for (auto &file : files)
	{
		if (file.first == oldName)
		{
			if (newName != "")
				file.first = newName;
			if (newPath != "")
				file.second = newPath;
			return;
		}
	}
}

void QuickStart::removeFile(const std::string &name)
{
	for (auto it = files.begin(); it != files.end(); it++)
	{
		if (it->first == name)
		{
			files.erase(it);
			return;
		}
	}
}