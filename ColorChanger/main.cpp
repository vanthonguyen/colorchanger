#include "colorchanger.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorChanger w;
    w.show();

    return a.exec();
}
