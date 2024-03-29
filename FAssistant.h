#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include "qevent.h"
#include "qlistwidget.h"
#include "ui_FAssistant.h"

class FAssistant : public QMainWindow
{
	Q_OBJECT

public:
	FAssistant(QWidget *parent = nullptr);
	~FAssistant();

	void makeConnection();
	void closeEvent(QCloseEvent *event);
	void loadPaths();
	inline void reload();

private:
	Ui::FAssistantClass ui;
};
