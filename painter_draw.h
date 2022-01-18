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

    QPoint startPoint;//鼠标点击开始绘制坐标,用于图形移动
    QPoint endPoint;//绘制结束坐标
    QPoint dragBegin;//拖拽开始坐标

    QTextEdit *tEdit;//文本输入框

    QVector<QVector<QPoint> > lines;//线条集合(一条线条可包含多个线段)
    QVector<QRect> rects;//矩形集合
    QVector<QRect> ellipse;//椭圆集合
    QVector<QRect>  line;//直线集合
    QVector<QString>  text;//文字集合
    QVector<QPoint>  tpoint;//文字位置集合
    QVector<int>  shape;//图形类型集合，用于撤回功能

    int openflag;//打开图片
    int drawType;//描绘类型
    int lpress = false;//左键按下标志
    int drag;//拖拽标志

    bool isDrawing = false;//是否正在画图标志


public slots:
    void Lines();//铅笔画线
    void SavePic();//保存图片
    void Rects();//画矩形
    void Ellipses();//画椭圆
    void Line();//画直线
    void OpenPic();//打开图片
    void Texts();//文字
    void AddTexts();//添加文字

};
#endif // PAINTER_DRAW_H
