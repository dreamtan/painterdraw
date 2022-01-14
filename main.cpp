#include "painter_draw.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    painter_draw w;
    w.show();
    return a.exec();
}
