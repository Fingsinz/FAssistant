#pragma once
#pragma execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include "ui_FAssistant.h"

class FAssistant : public QMainWindow
{
	Q_OBJECT

public:
	FAssistant(QWidget *parent = nullptr);
	~FAssistant();

	void makeConnection();

private:
	Ui::FAssistantClass ui;
};
