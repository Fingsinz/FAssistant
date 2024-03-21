#include "FAssistant.h"
#include "qdesktopservices.h"
#include "qdialogbuttonbox.h"
#include "qdir.h"
#include "qdiriterator.h"
#include "qfiledialog.h"
#include "qformlayout.h"
#include "qlineedit.h"
#include "qlistwidget.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "QuickStart.h"

extern QuickStart quickStart;

FAssistant::FAssistant(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/image/logo.jpg"));
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
				QDialog dialog(this);
				dialog.setWindowTitle("修改条目");

				QFormLayout layout(&dialog);
				QString inputName = "请输入名字：";
				QString inputPath = "请输入路径：";
				QLineEdit inputNameEdit;
				QLineEdit inputPathEdit;
				QPushButton pathButton("浏览", &dialog);
				connect(&pathButton, &QPushButton::clicked, &dialog,
					[this, &inputPathEdit, &inputNameEdit] {
						QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择路径"), "", tr("*.*"));
						inputPathEdit.setText(path);
						inputNameEdit.setText(path.section('/', -1, -1));
					});
				inputNameEdit.setText(curItem->text());
				inputPathEdit.setText(curItem->toolTip());
				QFormLayout selectPath(&dialog);
				selectPath.addRow(&inputPathEdit, &pathButton);
				layout.addRow(inputName, &inputNameEdit);
				layout.addRow(inputPath, &selectPath);

				QDialogButtonBox buttonBox;
				buttonBox.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
				QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
				QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
				buttonBox.button(QDialogButtonBox::Ok)->setText("修改");
				buttonBox.button(QDialogButtonBox::Cancel)->setText("取消");
				layout.addRow(&buttonBox);

				if (dialog.exec() == QDialog::Rejected)	return;
				quickStart.modifyFile(curItem->text().toStdString(), inputNameEdit.text().toStdString(), inputPathEdit.text().toStdString());
				reload();
				});
			popMenu->addAction("删除条目", this, [&] {
				quickStart.removeFile(curItem->text().toStdString());
				reload();
				});
		}

		popMenu->addAction("添加条目", this, [&] {
			QDialog dialog;
			dialog.setWindowTitle("添加条目");

			QFormLayout layout(&dialog);
			QString inputName = "请输入名字：";
			QString inputPath = "请输入路径：";
			QLineEdit inputNameEdit;
			QLineEdit inputPathEdit;
			QPushButton pathButton("浏览", &dialog);
			connect(&pathButton, &QPushButton::clicked, &dialog,
				[this, &inputPathEdit, &inputNameEdit] {
					QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择路径"), "", tr("*.*"));
					inputPathEdit.setText(path);
					inputNameEdit.setText(path.section('/', -1, -1));
				});
			QFormLayout selectPath(&dialog);
			selectPath.addRow(&inputPathEdit, &pathButton);
			layout.addRow(inputName, &inputNameEdit);
			layout.addRow(inputPath, &selectPath);

			QDialogButtonBox buttonBox;
			buttonBox.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
			QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
			QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
			buttonBox.button(QDialogButtonBox::Ok)->setText("添加");
			buttonBox.button(QDialogButtonBox::Cancel)->setText("取消");
			layout.addRow(&buttonBox);

			if (dialog.exec() == QDialog::Rejected)	return;
			quickStart.addFile(inputNameEdit.text().toStdString(), inputPathEdit.text().toStdString());
			reload();
			});

		popMenu->addAction("导入条目", this, [&] {
			QString fileName =
				QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择记录文件"), "", tr("*.fapps"));
			if (fileName.isEmpty()) return;
			quickStart.load(fileName.toStdString());
			quickStart.setRecordPath(fileName.toStdString());
			ui.statusBar->showMessage(fileName);
			reload();
			});

		popMenu->addAction("导出条目", this, [&] {
			QString fileName =
				QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存记录文件"), "", tr("*.fapps"));
			if (fileName.isEmpty()) return;
			quickStart.save(fileName.toStdString());
			});

		popMenu->exec(QCursor::pos());
		delete popMenu;
		});
}

void FAssistant::closeEvent(QCloseEvent *event)
{
	if (quickStart.getFiles().size() == 0) return;

	QDialog dialog;
	dialog.setWindowTitle("关闭");
	QFormLayout layout(&dialog);

	QString tips = "是否保存当前条目记录？";
	QLabel label(tips);

	QDialogButtonBox buttonBox;
	buttonBox.setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	buttonBox.button(QDialogButtonBox::Ok)->setText("保存");
	buttonBox.button(QDialogButtonBox::Cancel)->setText("不保存");

	layout.addRow(&label);
	layout.addRow(&buttonBox);

	if (dialog.exec() == QDialog::Accepted)	quickStart.save(quickStart.getRecordPath());
}

void FAssistant::loadPaths()
{
	QString fappsPath;
	QDir dir;
	dir.setNameFilters(QStringList() << "*.fapps");

	QDirIterator iter(dir, QDirIterator::NoIteratorFlags);
	while (iter.hasNext())
	{
		iter.next();
		QFileInfo info = iter.fileInfo();
		if (info.isFile())
		{
			fappsPath = info.absoluteFilePath();
			ui.statusBar->showMessage(fappsPath);
			break;
		}
	}

	if (fappsPath.isEmpty())
	{
		fappsPath = QDir::currentPath() + "/default.fapps";
		ui.statusBar->showMessage(fappsPath);
		QFile file(fappsPath);
		file.open(QFile::WriteOnly);
		file.close();
	}

	quickStart.setRecordPath(fappsPath.toStdString());

	if (!quickStart.load(fappsPath.toStdString()))
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