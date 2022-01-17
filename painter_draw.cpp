#include "painter_draw.h"
#include "ui_painter_draw.h"

painter_draw::painter_draw(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::painter_draw)
{
    ui->setupUi(this);

    //初始化
    lpress = false;
    setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听
    pix = QPixmap(600,400);
    pix.fill(Qt::white);
    tempPix = pix;
    isDrawing = false;
    this->setGeometry(350,200,600,400);//设置窗体位置、大小
    this->setStyleSheet("background_color:white;");//设置背景色

    //创建工具栏
    QToolBar *tbar = addToolBar(tr("工具栏"));
    tbar->setMovable(false);//工具栏不可移动
    tbar->setIconSize(QSize(16, 16));//设置动作图标的尺寸
    tbar->setStyleSheet("background-color:rgb(199,237,204)");//背景色
}

//鼠标事件
void painter_draw::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        //当鼠标左键按下时获取当前位置作为矩形开始点
        startPoint = event->pos();
        //标记正在绘图
        isDrawing = true;
    }
}
void painter_draw::mouseMoveEvent(QMouseEvent *event){
    if(event->button()&Qt::LeftButton){
        //当按着左键移动时，获取当前位置作为结束点，绘制矩形
        endPoint = event->pos();
        //将缓冲区内容复制到临时缓冲区，动态绘制时在缓冲区上绘制。不产生拖影
        tempPix = pix;
    }
}
void painter_draw::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        //左键松开，绘画结束，获取结束点
        endPoint = event->pos();
        //标记结束绘画
        isDrawing = false;
        update();
    }
}
//绘画事件
void painter_draw::paintEvent(QPaintEvent *event){

    int x = startPoint.x();
    int y = startPoint.y();
    int width = endPoint.x() - x;
    int heigh = endPoint.y() - y;
    QPainter painter;
    painter.begin(&tempPix);
    painter.drawRect(x,y,width,heigh);
    painter.end();
    painter.begin(this);
    painter.drawPixmap(0,0,tempPix);
    //完成绘制，更新缓冲区
    if(!isDrawing){
        pix = tempPix;
    }

}
painter_draw::~painter_draw()
{
    delete ui;
}

