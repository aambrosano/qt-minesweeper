#include "minewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MineWindow w;
    w.show();

    return a.exec();
}
