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
    //右键菜单事件
    void contextMenuEvent(QContextMenuEvent *);//重写菜单事件
private:
    Ui::painter_draw *ui;

    QMenu *menu;//右键菜单

    QPixmap pix;//缓冲区
    QPixmap tempPix;//临时缓冲区

    QPoint startPoint;//鼠标点击开始绘制坐标
    QPoint endPoint;//绘制结束坐标
    QPoint dragBegin;//拖拽开始坐标



    int drawType;//描绘类型
    int lpress = false;//左键按下标志
    int drag;//拖拽标志

    bool isDrawing = false;//是否正在画图标志


};
#endif // PAINTER_DRAW_H
