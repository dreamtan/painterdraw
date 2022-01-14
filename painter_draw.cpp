#include "painter_draw.h"
#include "ui_painter_draw.h"

painter_draw::painter_draw(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::painter_draw)
{
    ui->setupUi(this);
}

painter_draw::~painter_draw()
{
    delete ui;
}

