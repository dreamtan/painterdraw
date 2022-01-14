#ifndef PAINTER_DRAW_H
#define PAINTER_DRAW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class painter_draw; }
QT_END_NAMESPACE

class painter_draw : public QMainWindow
{
    Q_OBJECT

public:
    painter_draw(QWidget *parent = nullptr);
    ~painter_draw();

private:
    Ui::painter_draw *ui;
};
#endif // PAINTER_DRAW_H
