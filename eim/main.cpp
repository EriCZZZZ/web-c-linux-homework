#include "imwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IMWindow w;
    w.show();

    return a.exec();
}
