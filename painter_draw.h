#ifndef PAINTER_DRAW_H
#define PAINTER_DRAW_H



#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QToolBar>
#include <QAction>
#include <QPalette>
#include <QColor>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class painter_draw; }
QT_END_NAMESPACE

class painter_draw : public QMainWindow
{
    Q_OBJECT
public:
    painter_draw(QWidget *parent = nullptr);
    ~painter_draw();
protected:
    void paintEvent(QPaintEvent *event);//重写窗体重绘
    //鼠标事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::painter_draw *ui;

    QPixmap pix;//缓冲区
    QPixmap tempPix;//临时缓冲区

    QPoint startPoint;
    QPoint endPoint;

    int drawType;//描绘类型
    int lpress = false;//左键按下标志
    int drag;//拖拽标志

    bool isDrawing = false;//是否正在画图标志


};
#endif // PAINTER_DRAW_H
