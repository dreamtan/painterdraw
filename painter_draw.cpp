#include "painter_draw.h"
#include "ui_painter_draw.h"

painter_draw::painter_draw(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::painter_draw)
{
    ui->setupUi(this);

    //初始化
    lpress = false;
    drawType = 0;//初始绘画类型无
    drag = 0;//初始非拖拽
    dragBegin = pos();//拖拽参考坐标，计算位移
    setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听

    openflag = 0;//初始不打开图片

    //this->setGeometry(350,200,600,400);//设置窗体位置、大小
    this->setStyleSheet("background_color:white;");//设置背景色

    tEdit = new QTextEdit(this);//初始化文本输入框
    tEdit->hide();//隐藏

    //创建工具栏
    QToolBar *tbar = addToolBar(tr("工具栏"));
    tbar->setMovable(false);//工具栏不可移动
    tbar->setIconSize(QSize(16, 16));//设置动作图标的尺寸
    tbar->setStyleSheet("background-color:rgb(199,237,204)");//背景色
    //右键菜单
    menu = new QMenu(this);
    menu->addAction("保存 \tCtrl+S",this,SLOT(SavePic()));//添加保存动作
    menu->addAction("退出 \tALT+F4",this,SLOT(close()));//添加关闭动作
    menu->setStyleSheet("background-color:rgb(199,237,204)");//设置背景色

    QAction *openAction = new QAction("&打开",this);
    openAction->setShortcut(QKeySequence("Ctrl+O"));
    tbar->addAction(openAction);//添加到工具栏

    QAction *saveAction = new QAction("&保存", this);//保存动作
    saveAction->setShortcut(QKeySequence("Ctrl+S"));//热键
    tbar->addAction(saveAction);//添加到工具栏

    QAction *saveasAction = new QAction(tr("&另存为"), this);//保存动作
    saveasAction->setShortcut(QKeySequence("Ctrl+ALT+S"));//热键
    tbar->addAction(saveasAction);//添加到工具栏

    QAction *lineAction = new QAction(tr("&直线"), this);//直线动作
    lineAction->setShortcut(QKeySequence("Ctrl+L"));//热键
    tbar->addAction(lineAction);//添加到工具栏

    QAction *linesAction = new QAction(tr("&铅笔"), this);//铅笔动作
    linesAction->setShortcut(QKeySequence("Ctrl+P"));//热键
    tbar->addAction(linesAction);//添加到工具栏

    QAction *rectAction = new QAction(tr("&矩形"), this);//矩形动作
    rectAction->setShortcut(QKeySequence("Ctrl+R"));//热键
    tbar->addAction(rectAction);

    QAction *ellipseAction = new QAction(tr("&椭圆"), this);//椭圆动作
    ellipseAction->setShortcut(QKeySequence("Ctrl+E"));//热键
    tbar->addAction(ellipseAction);

    QAction *textAction = new QAction(tr("&文字"), this);//文字动作
    textAction->setShortcut(QKeySequence("Ctrl+T"));//热键
    tbar->addAction(textAction);

    //连接信号与槽函数
    connect(linesAction, SIGNAL(triggered()), this, SLOT(Lines()));
    connect(rectAction, SIGNAL(triggered()), this, SLOT(Rects()));
    connect(ellipseAction, SIGNAL(triggered()), this, SLOT(Ellipses()));
    connect(lineAction, SIGNAL(triggered()), this, SLOT(Line()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SavePic()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(OpenPic()));
    connect(textAction, SIGNAL(triggered()), this, SLOT(Texts()));
    connect(tEdit, SIGNAL(textChanged()), this, SLOT(AddTexts()));

}

//鼠标事件
void painter_draw::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        switch (drawType) {
        case 1:{//铅笔
            lpress = true;//左键按下
            QVector<QPoint> line;
            lines.append(line);
            QVector<QPoint> &lastLine = lines.last();
            lastLine.append(event->pos());
            shape.append(1);
            break;
        }
        case 2:{//矩形
            lpress = true;//左键
            if(!drag){//非拖拽模式
                QRect rect;
                rects.append(rect);
                QRect &lastRect = rects.last();
                lastRect.setTopLeft(event->pos());//矩形左上角坐标
                shape.append(2);
            }else if (rects.last().contains(event->pos())) {//拖拽模式
                    dragBegin = event->pos();
             }
        break;}
        case 3:{//椭圆
            lpress = true;//左键
            if(!drag){//非拖拽模式
                QRect rect;
                ellipse.append(rect);
                QRect &lastEllipse = ellipse.last();
                lastEllipse.setTopLeft(event->pos());//椭圆左上角坐标
                shape.append(3);
            }else if (ellipse.last().contains(event->pos())) {//拖拽模式
                    dragBegin = event->pos();
             }
        break;
        }
        case 4:{//直线
            lpress = true;//左键按下标志
            QRect rect;//鼠标按下，直线一端开始
            line.append(rect);//将新直线添加到直线集合
            QRect& lastLine = line.last();//拿到新直线
            lastLine.setTopLeft(event->pos());//记录鼠标的坐标(新直线开始一端坐标)
            shape.append(4);
            break;
        }
        case 5:{//文字
            update();//窗体重绘
            QPoint p;//鼠标按下，文字开始
            tpoint.append(p);//文字坐标
            QPoint &lastp = tpoint.last();
            lastp = event->pos();
            tEdit->setGeometry(lastp.x()-6,lastp.y()-17,90,90);//文本框位置大小
            tEdit->show();//显示文本输入框
            text.append("");
            tEdit->clear();//全局只有一个，使用前清空
            shape.append(5);
            break;
        }
        }
    }
}
void painter_draw::mouseMoveEvent(QMouseEvent *event){
    if(drag && ((drawType == 2 && rects.last().contains(event->pos()))
                || (drawType == 3 && ellipse.last().contains(event->pos()) )))
        {
            setCursor(Qt::SizeAllCursor);//拖拽模式下，并且在拖拽图形中，将光标形状改为十字
        }else
        {
            setCursor(Qt::ArrowCursor);//恢复原始光标形状
            drag = 0;
        }

    if(lpress){
        //当按着左键移动时，获取当前位置作为结束点，绘制矩形
        switch (drawType) {
        case 1:{//铅笔
            if(lines.size()<=0)return;//线条集合为空，不画线
            QVector<QPoint> &lastLine = lines.last();
            lastLine.append(event->pos());//记录鼠标坐标（线条轨迹）
            update();//重绘
            break;
        }
        case 2:{//矩形
            //if(drag==0){//非拖拽模式
                QRect &lastRect = rects.last();//拿到新矩形
                lastRect.setBottomRight(event->pos());//矩形右下角坐标
//            }else{//拖拽模式
//                QRect &lastRect = rects.last();
//                if(lastRect.contains(event->pos())){//在矩形的内部
//                      int dx = event->pos().x()-dragBegin.x();//横向移动x
//                      int dy = event->pos().y()-dragBegin.y();//纵向移动y
//                      lastRect = lastRect.adjusted(dx,dy,dx,dy);//更新矩形的位置
//                      dragBegin = event->pos();//刷新拖拽点起始坐标
//                  }
//             }
            update();
        break;}
        case 3:{//椭圆
           // if(drag==0){//非拖拽模式
                QRect &lastEllipse = ellipse.last();
                lastEllipse.setBottomRight(event->pos());//椭圆左上角坐标
//            }else{//拖拽模式
//                QRect &lastEllipse = ellipse.last();
//                if(lastEllipse.contains(event->pos())){//椭圆内部
//                    int dx = event->pos().x()-dragBegin.x();//横向移动x
//                    int dy = event->pos().y()-dragBegin.y();//纵向移动y
//                    lastEllipse = lastEllipse.adjusted(dx,dy,dx,dy);
//                    dragBegin = event->pos();
//                }
//             }
            update();
        break;
        }
        case 4:{//直线
            QRect &lastLine = line.last();//拿到新直线
            lastLine.setBottomRight(event->pos());//更新直线另一端)
            update();//触发窗体重绘
            break;
        }
        }
    }
}
void painter_draw::mouseReleaseEvent(QMouseEvent *event){
    if(lpress){
        switch (drawType) {
        case 1:{//线条
            QVector<QPoint> &lastLine = lines.last();
            lastLine.append(event->pos());
            lpress = false;
            break;
        }
        case 2:{//矩形
            QRect &lastRect = rects.last();
           // if(!drag){//非拖拽模式
                lastRect.setBottomRight(event->pos());//
                //刚画完矩形，讲光标设置到新矩形的中心位置，并进入拖拽模式
//                this->cursor().setPos(this->cursor().pos().x()-lastRect.width()/2,this->cursor().pos().y()-lastRect.height()/2);
//                drag = 1;
//            }
            lpress = false;
        break;}
        case 3:{//椭圆
            QRect& lastEllipse = ellipse.last();//拿到新椭圆
 //           if(!drag){
                lastEllipse.setBottomRight(event->pos());//不是拖拽时，更新椭圆的右下角坐标)
                //刚画完椭圆，将光标设置到新椭圆的中心位置，并进入拖拽模式
//                this->cursor().setPos(this->cursor().pos().x()-lastEllipse.width()/2,this->cursor().pos().y()-lastEllipse.height()/2);
//                drag = 1;
//            }
            lpress = false;
        break;
        }
        case 4:{//直线
            QRect& lastLine = line.last();//
            lastLine.setBottomRight(event->pos());//
            lpress = false;
            break;
        }
        }
    }
}
//右键菜单事件
void painter_draw::contextMenuEvent(QContextMenuEvent *)  //右键菜单事件
{
    menu->exec(cursor().pos());//在光标位置弹出菜单
}
//文本框
void painter_draw::AddTexts(){
    QString &last = text.last();//拿到最后一个文本
    last = tEdit->toPlainText();
}
//绘画事件
void painter_draw::paintEvent(QPaintEvent *){
    if(openflag == 0){//不是打开图片的情况下新建空白画布
        pixmap = QPixmap(size());//新建画布
        pixmap.fill(Qt::white);//背景
    }
    QPixmap pix = pixmap;
    QPainter painter(&pix);//以pixmap作为画布
    unsigned int i1=0,i2=0,i3=0,i4=0,i5=0;//各种图形的索引
    for (int c = 0;c<shape.size();++c) {
        switch (shape.at(c)) {
            case 1:{
               const QVector<QPoint>& line = lines.at(i1++);//取出一条线条,at返回i处的项//at(),operator(),value()区别，at必须是有效索引值，operator值可该，value值不变
               for(int j = 0;j<line.size()-1;++j){//将线条所有线段描绘出
                    painter.drawLine(line.at(j),line.at(j+1));
               }
            break;}
            case 2:painter.drawRect(rects.at(i2++));//矩形
            break;
            case 3:painter.drawEllipse(ellipse.at(i3++));//椭圆
            break;
            case 4:{//直线
                painter.drawLine(line.at(i4).topLeft(),line.at(i4).bottomRight());
                i4++;
            break;}
            case 5:{//文本
                painter.drawText(tpoint.at(i5),text.at(i5));
                i5++;
            break;
            }
        }
    }
    painter.end();
    painter.begin(this);
    painter.drawPixmap(0,0,pix);//画到窗体

}

void painter_draw::Lines()
{
    drawType = 1;//铅笔
    tEdit->hide();//文本框隐藏
}

void painter_draw::Line()
{
    drawType = 4;//直线
    tEdit->hide();
}

void painter_draw::Rects()
{
    drawType = 2;//矩形
    tEdit->hide();
}

void painter_draw::Ellipses()
{
    drawType = 3;//椭圆
    tEdit->hide();
}

void painter_draw::Texts()
{
    drawType = 5;//文字
}

void painter_draw::SavePic()
{
    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new.jpg", "Image (*.jpg *.png *.bmp)");

    if (fileName.length() > 0)
    {
        tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(0,30,size().width(),size().height()-30)).save(fileName);//不包含工具栏
    }
}

void painter_draw::OpenPic()
{
    //弹出文件打开对话框
    QString picPath = QFileDialog::getOpenFileName(this,tr("打开"),"","Image Files(*.jpg *.png)");
    if(!picPath.isEmpty())//用户选择了文件
    {
        QPixmap pix;
        pix.load(picPath);//加载图片
        QPainter p(&pixmap);
        p.drawPixmap(0,30,pix);//添加工具栏的空间
        openflag = 1;//设置文件打开标志
        update();//触发窗体重绘，将图片画到窗体
    }
}
void painter_draw::keyPressEvent(QKeyEvent *event) //按键事件
{
     //Ctrl+Z撤销
     if (event->key() == Qt::Key_Z && event->modifiers() == Qt::ControlModifier)
     {
         if(shape.size()>0)
         {
             switch(shape.last())
             {
                 case 1: lines.pop_back();
                         break;
                 case 2: rects.pop_back();
                         break;
                 case 3: ellipse.pop_back();
                         break;
                 case 4: line.pop_back();
                         break;
             }
             shape.pop_back();
             drag = 0;//设置为非拖拽模式
             update();
         }
     }
     else if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier)//保存
     {
        SavePic();//Ctrl+S保存
     }
}
painter_draw::~painter_draw()
{
    delete ui;
}
