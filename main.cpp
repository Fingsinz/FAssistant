#include "FAssistant.h"
#include "QuickStart.h"
#include <QtWidgets/QApplication>

QuickStart quickStart;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FAssistant w;
	w.show();
	return a.exec();
}