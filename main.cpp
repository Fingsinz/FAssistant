#include "FAssistant.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FAssistant w;
    w.show();
    return a.exec();
}
