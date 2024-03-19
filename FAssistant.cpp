#include "FAssistant.h"
#include "QuickStart.h"
#include "qlistwidget.h"
#include "qdir.h"
#include "qfiledialog.h"
#include "qdesktopservices.h"
#include "qmessagebox.h"

FAssistant::FAssistant(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	loadPaths();
	makeConnection();
}

FAssistant::~FAssistant()
{}

void FAssistant::makeConnection()
{
	// °ó¶¨Ë«»÷´ò¿ª
	connect(ui.list, &QListWidget::itemDoubleClicked, [this] (QListWidgetItem *item) {
		QDesktopServices::openUrl(QUrl(item->data(Qt::UserRole).toString()));
		});
}

void FAssistant::loadPaths()
{
	QuickStart q;
	QFile file;
	if (!q.load("test.fapps"))
		return;

	ui.list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	for (auto &file : q.getFiles())
	{
		QListWidgetItem *item = new QListWidgetItem(file.first.c_str());
		item->setData(Qt::UserRole, file.second.c_str());
		item->setToolTip(file.second.c_str());
		ui.list->addItem(item);
	}
}