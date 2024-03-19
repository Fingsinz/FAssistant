﻿#include "FAssistant.h"
#include "qdesktopservices.h"
#include "qdialogbuttonbox.h"
#include "qdir.h"
#include "qfiledialog.h"
#include "qformlayout.h"
#include "qlineedit.h"
#include "qlistwidget.h"
#include "qmessagebox.h"
#include "QuickStart.h"

extern QuickStart quickStart;

FAssistant::FAssistant(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.list->setContextMenuPolicy(Qt::CustomContextMenu);
	loadPaths();
	makeConnection();
}

FAssistant::~FAssistant()
{}

void FAssistant::makeConnection()
{
	// 绑定双击打开
	connect(ui.list, &QListWidget::itemDoubleClicked, [this] (QListWidgetItem *item) {
		QDesktopServices::openUrl(QUrl(item->data(Qt::UserRole).toString()));
		});

	connect(ui.list, &QListWidget::customContextMenuRequested, [this] (const QPoint &pos) {
		QMenu *popMenu = new QMenu(this);
		QListWidgetItem *curItem = ui.list->itemAt(pos);

		if (curItem != nullptr)
		{
			popMenu->addAction("修改条目", this, [&] {
				QDialog dialog;
				dialog.setWindowTitle("修改条目");

				QFormLayout layout(&dialog);
				QString inputName = "请输入名字：";
				QString inputPath = "请输入路径：";
				QLineEdit inputNameEdit;
				QLineEdit inputPathEdit;
				inputNameEdit.setText(curItem->text());
				inputPathEdit.setText(curItem->toolTip());
				layout.addRow(inputName, &inputNameEdit);
				layout.addRow(inputPath, &inputPathEdit);

				QDialogButtonBox buttonBox;
				buttonBox.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
				QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
				QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
				layout.addRow(&buttonBox);

				if (dialog.exec() == QDialog::Rejected)	return;
				quickStart.modifyFile(curItem->text().toStdString(), inputNameEdit.text().toStdString(), inputPathEdit.text().toStdString());
				reload();
				});
			popMenu->addAction("删除条目", this, [&] {});
		}

		else
		{
			popMenu->addAction("添加条目", this, [&] {});
		}

		popMenu->addAction("导入条目", this, [&] {
			QString fileName =
				QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择记录文件"), "", tr("*.fapps"));
			quickStart.load(fileName.toStdString());
			reload();
			});

		popMenu->addAction("导出条目", this, [&] {
			QString fileName =
				QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存记录文件"), "", tr("*.fapps"));
			quickStart.save(fileName.toStdString());
			});

		popMenu->exec(QCursor::pos());
		delete popMenu;
		});
}

void FAssistant::loadPaths()
{
	QDir dir;
	ui.clipBoard->append(dir.path());
	for (auto &file : dir.entryList(QStringList() << "*.fapps", QDir::Files))
		ui.clipBoard->append(dir.absoluteFilePath(file));

	if (!quickStart.load("test.fapps"))
		return;

	ui.list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	reload();
}

void FAssistant::reload()
{
	ui.list->clear();
	for (auto &file : quickStart.getFiles())
	{
		QListWidgetItem *item = new QListWidgetItem(file.first.c_str());
		item->setData(Qt::UserRole, file.second.c_str());
		item->setToolTip(file.second.c_str());
		ui.list->addItem(item);
	}
}