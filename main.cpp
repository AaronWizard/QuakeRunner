#include "view/quakerunner.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/icon.png"));

    QuakeRunner w;
    w.show();
    return a.exec();
}
