#include "view/quakerunner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QuakeRunner w;
    w.show();
    return a.exec();
}
