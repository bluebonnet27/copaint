#include "mypaint.h"
#include <QDebug>
#include <iostream>
MyPaint::MyPaint(QWidget *parent) :
    QMainWindow(parent)
{
     _lpress = false;//初始鼠标左键未按下
     _drawType = 0;//初始为什么都不画
     _drag = 0;//默认非拖拽模式
     _begin = pos();//拖拽的参考坐标，方便计算位移
     _openflag = 0;//初始不打开图片
     _tEdit = new QTextEdit(this);//初始化文本输入框
     _tEdit->hide();//隐藏
     _brush = Qt::white;
     this->setGeometry(120,80,1280,720);//设置窗体大小、位置
     setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听
     this->setStyleSheet("background-color:white;");

    //创建菜单栏
    QMenuBar *tMenuBar = menuBar();

    //文件
    QMenu *fileMenu = new QMenu(QStringLiteral("文件(&F)"));
    QMenu *paintMenu = new QMenu(QStringLiteral("绘图(&P)"));
    QMenu *colorMenu = new QMenu(QStringLiteral("颜色(&S)"));
    QMenu *coopMenu = new QMenu(QStringLiteral("合作(&C)"));
    QMenu *helpMenu = new QMenu(QStringLiteral("帮助(&H)"));
    QMenu *aboutMenu = new QMenu(QStringLiteral("关于(&A)"));

    //创建工具栏
    QToolBar *tbar = addToolBar(tr("工具栏"));
    tbar->setMovable(false);//工具栏不可移动
    tbar->setIconSize(QSize(25, 25));//设置动作图标的尺寸
    tbar->setStyleSheet("background-color:rgb(225,245,254)");//背景色
    //tbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    _Rmenu = new QMenu(this);//创建右键菜单
    _Rmenu->addAction(QIcon(":/png/images/refresh.png"),tr("刷新  \tF4"), this, SLOT());//添加菜单动作
    _Rmenu->addAction(QIcon(":/png/images/copy.png"),tr("复制  \tCtrl+C"), this, SLOT(SetPicToClipboard()));//添加菜单动作
    _Rmenu->addAction(QIcon(":/png/images/paste.png"),tr("粘贴  \tCtrl+V"), this, SLOT(GetPicFromClipboard()));//添加菜单动作
    _Rmenu->addAction(QIcon(":/png/images/save.png"),tr("保存  \tCtrl+S"), this, SLOT(SavePic()));//添加菜单动作
    _Rmenu->addAction(QIcon(":/png/images/undo.png"),tr("撤销操作 \tCtrl+Z"),this,SLOT(UndoPic()));
    _Rmenu->addAction(QIcon(":/png/images/quit.png"),tr("退出  \tALT+F4"), this, SLOT(close()));//添加菜单动作
    _Rmenu->addAction(QIcon(":/png/images/help.png"),tr("帮助  \tCtrl+H"), this, SLOT(GetHelp()));//添加菜单动作
    _Rmenu->setStyleSheet("background-color:rgb(33,150,243)");//设置背景色

    QAction *openAction = new QAction(tr("&打开"), this);//打开动作
    openAction->setIcon(QIcon(":/png/images/open.png"));//图标
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));//热键
    tbar->addAction(openAction);//添加到工具栏
    fileMenu->addAction(openAction);

    QAction *saveAction = new QAction(tr("&保存"), this);//保存动作
    saveAction->setIcon(QIcon(":/png/images/save.png"));//图标
    saveAction->setShortcut(QKeySequence(tr("Ctrl+S")));//热键
    tbar->addAction(saveAction);//添加到工具栏
    fileMenu->addAction(saveAction);

    QAction *saveasAction = new QAction(tr("&另存为"), this);//另存为动作
    saveasAction->setIcon(QIcon(":/png/images/saveas.png"));//图标
    saveasAction->setShortcut(QKeySequence(tr("Ctrl+ALT+S")));//热键
    tbar->addAction(saveasAction);//添加到工具栏
    fileMenu->addAction(saveasAction);

    QAction *cursorAction = new QAction(tr("&指针"), this);//直线动作
    cursorAction->setIcon(QIcon(":/png/images/cursor.png"));//图标
    tbar->addAction(cursorAction);//添加到工具栏

    QAction *lineAction = new QAction(tr("&直线"), this);//直线动作
    lineAction->setIcon(QIcon(":/png/images/line.png"));//图标
    lineAction->setShortcut(QKeySequence(tr("Ctrl+L")));//热键
    tbar->addAction(lineAction);//添加到工具栏
    paintMenu->addAction(lineAction);

    QAction *linesAction = new QAction(tr("&铅笔"), this);//铅笔动作
    linesAction->setIcon(QIcon(":/png/images/lines.png"));//图标
    linesAction->setShortcut(QKeySequence(tr("Ctrl+P")));//热键
    tbar->addAction(linesAction);//添加到工具栏
    paintMenu->addAction(linesAction);

    QAction *rectAction = new QAction(tr("&矩形"), this);//矩形动作
    rectAction->setIcon(QIcon(":/png/images/rect.png"));//图标
    rectAction->setShortcut(QKeySequence(tr("Ctrl+R")));//热键
    tbar->addAction(rectAction);
    paintMenu->addAction(rectAction);

    QAction *roundRectAction = new QAction(tr("&圆角矩形"), this);//矩形动作
    roundRectAction->setIcon(QIcon(":/png/images/roundrect.png"));//图标
    roundRectAction->setShortcut(QKeySequence(tr("Ctrl+I")));//热键
    tbar->addAction(roundRectAction);
    paintMenu->addAction(roundRectAction);

    QAction *ellipseAction = new QAction(tr("&椭圆"), this);//椭圆动作
    ellipseAction->setIcon(QIcon(":/png/images/ellipse.png"));//图标
    ellipseAction->setShortcut(QKeySequence(tr("Ctrl+E")));//热键
    tbar->addAction(ellipseAction);
    paintMenu->addAction(ellipseAction);

    QAction *textAction = new QAction(tr("&文字"), this);//文字动作
    textAction->setIcon(QIcon(":/png/images/texts.png"));//图标
    textAction->setShortcut(QKeySequence(tr("Ctrl+T")));//热键
    tbar->addAction(textAction);
    paintMenu->addAction(textAction);

    QAction *copyAction = new QAction(tr("&复制"), this);//文字动作
    copyAction->setIcon(QIcon(":/png/images/copy.png"));//图标
    copyAction->setShortcut(QKeySequence(tr("Ctrl+C")));//热键
    tbar->addAction(copyAction);
    fileMenu->addAction(copyAction);

    QAction *pasteAction = new QAction(tr("&粘贴"), this);//文字动作
    pasteAction->setIcon(QIcon(":/png/images/paste.png"));//图标
    pasteAction->setShortcut(QKeySequence(tr("Ctrl+V")));//热键
    tbar->addAction(pasteAction);
    fileMenu->addAction(pasteAction);

    QAction *coopAction = new QAction(tr("&合作"),this);
    coopAction->setIcon(QIcon(":/png/images/coop.png"));
    coopAction->setShortcut(QKeySequence(tr("Ctrl+E")));
    tbar->addAction(coopAction);
    coopMenu->addAction(coopAction);

    QAction *helpAction = new QAction(tr("&帮助"),this);
    helpAction->setIcon(QIcon(":/png/images/help.png"));
    helpAction->setShortcut(QKeySequence(tr("Ctrl+H")));
    tbar->addAction(helpAction);
    helpMenu->addAction(helpAction);

    QAction *aboutAction = new QAction(tr("&关于"),this);
    aboutAction->setIcon(QIcon(":/png/images/about.png"));
    aboutAction->setShortcut(QKeySequence(tr("Ctrl+A")));
    tbar->addAction(aboutAction);
    aboutMenu->addAction(aboutAction);

    QAction *redBrushAction = new QAction(tr("&红色画笔"),this);
    redBrushAction->setIcon(QIcon(":/png/images/red.png"));
    tbar->addAction(redBrushAction);
    colorMenu->addAction(redBrushAction);

    QAction *blueBrushAction = new QAction(tr("&蓝色画笔"),this);
    blueBrushAction->setIcon(QIcon(":/png/images/blue.png"));
    tbar->addAction(blueBrushAction);
    colorMenu->addAction(blueBrushAction);

    QAction *blackBrushAction = new QAction(tr("&黑色画笔"),this);
    blackBrushAction->setIcon(QIcon(":/png/images/black.png"));
    tbar->addAction(blackBrushAction);
    colorMenu->addAction(blackBrushAction);

    QAction *undoAction = new QAction(tr("&撤销"), this);//打开动作
    undoAction->setIcon(QIcon(":/png/images/undo.png"));//图标
    tbar->addAction(undoAction);//添加到工具栏

    QAction *deleteAllAction = new QAction(tr("&删除所有"),this);
    deleteAllAction->setIcon(QIcon(":/png/images/delete.png"));
    tbar->addAction(deleteAllAction);

    QAction *contactAction = new QAction(tr("&联系作者"),this);
    contactAction->setIcon(QIcon(":/png/images/email.png"));
    tbar->addAction(contactAction);
    helpMenu->addAction(contactAction);

    QAction *visitAction = new QAction(tr("&访问作者主页"),this);
    visitAction->setIcon(QIcon(":/png/images/internet.png"));
    tbar->addAction(visitAction);
    helpMenu->addAction(visitAction);

    QAction *visitProjectAction = new QAction(tr("&访问项目GitHub主页"),this);
    visitProjectAction->setIcon(QIcon(":/png/images/github.png"));
    tbar->addAction(visitProjectAction);
    helpMenu->addAction(visitProjectAction);

    QAction *exitAction = new QAction(tr("退出"),this);
    exitAction->setIcon(QIcon(":/png/images/quit.png"));
    tbar->addAction(exitAction);

    tMenuBar->addMenu(fileMenu);
    tMenuBar->addMenu(paintMenu);
    tMenuBar->addMenu(colorMenu);
    tMenuBar->addMenu(coopMenu);
    tMenuBar->addMenu(helpMenu);
    tMenuBar->addMenu(aboutMenu);

    //连接信号与槽函数
    //打开
    connect(openAction, SIGNAL(triggered()), this, SLOT(OpenPic()));
    //保存
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SavePic()));
    //另存为
    connect(saveasAction, SIGNAL(triggered()), this, SLOT(SaveAsPic()));
    //指针
    connect(cursorAction, SIGNAL(triggered()), this, SLOT(PointCursor()));
    //直线
    connect(lineAction, SIGNAL(triggered()), this, SLOT(Line()));
    //铅笔
    connect(linesAction, SIGNAL(triggered()), this, SLOT(Lines()));
    //矩形
    connect(rectAction, SIGNAL(triggered()), this, SLOT(Rects()));
    //圆角矩形
    connect(roundRectAction, SIGNAL(triggered()), this, SLOT(RoundRects()));
    //椭圆
    connect(ellipseAction, SIGNAL(triggered()), this, SLOT(Ellipses()));
    //文字创立
    connect(textAction, SIGNAL(triggered()), this, SLOT(Texts()));
    //文字输入
    connect(_tEdit, SIGNAL(textChanged()), this, SLOT(AddTexts()));
    //复制
    connect(copyAction,SIGNAL(triggered()),this,SLOT(SetPicToClipboard()));
    //粘贴
    connect(pasteAction,SIGNAL(triggered()),this,SLOT(GetPicFromClipboard()));
    //合作
    connect(coopAction,SIGNAL(triggered()),this,SLOT(ErrorFunction()));
    //帮助
    connect(helpAction,SIGNAL(triggered()),this,SLOT(GetHelp()));
    //关于
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(GetAbout()));
    //Contact the author
    connect(contactAction,SIGNAL(triggered()),this,SLOT(EmailToAuthor()));
    //delete all
    connect(deleteAllAction,SIGNAL(triggered()),this,SLOT(ClearAll()));
    //visit main page
    connect(visitAction,SIGNAL(triggered()),this,SLOT(VisitBlog()));
    //visit github
    connect(visitProjectAction,SIGNAL(triggered()),this,SLOT(VisitGithub()));
    //画笔
    connect(redBrushAction,SIGNAL(triggered()),this,SLOT(RedBrush()));
    connect(blueBrushAction,SIGNAL(triggered()),this,SLOT(BlueBrush()));
    connect(blackBrushAction,SIGNAL(triggered()),this,SLOT(BlackBrush()));
    //撤销
    connect(undoAction,SIGNAL(triggered()),this,SLOT(UndoPic()));
    //exit
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    //状态栏
    QStatusBar *pstatusBar = statusBar();

    pstatusLabel->setMinimumSize(pstatusLabel->sizeHint());
    pstatusLabel->setAlignment(Qt::AlignHCenter);

    pixLabel->setMinimumSize(pixLabel->sizeHint());
    pixLabel->setAlignment(Qt::AlignHCenter);

    pstatusBar->addWidget(pstatusLabel);
    pstatusBar->addWidget(pixLabel);
}

void MyPaint::paintEvent(QPaintEvent *)
{
    if(_openflag == 0)//不是打开图片的，每一次新建一个空白的画布
    {
        _pixmap = QPixmap(size());//新建pixmap
        _pixmap.fill(Qt::white);//背景色填充为白色
    }
    QPixmap pix = _pixmap;//以_pixmap作为画布
    QPainter p(&pix);//将_pixmap作为画布
    QPen pn;
    unsigned int i1=0,i2=0,i3=0,i4=0,i5=0,i6=0;//各种图形的索引

    for(int c = 0;c<_shape.size();++c)//控制用户当前所绘图形总数
    {
        //p.setBrush(Qt::white);
        if(_shape.at(c) == 1)//线条
        {
              const QVector<QPoint>& line = _lines.at(i1++);//取出一条线条
              for(int j=0; j<line.size()-1; ++j)//将线条的所有线段描绘出
              {
                  p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//反锯齿
                  p.drawLine(line.at(j), line.at(j+1));
              }
        }
        else if(_shape.at(c) == 2)//矩形
        {
             p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//反锯齿
             //p.setBrush(_brush);
             p.drawRect(_rects.at(i2++));
        }
        else if(_shape.at(c) == 3)//椭圆
        {
            p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//反锯齿
            p.drawEllipse(_ellipse.at(i3++));
        }
        else if(_shape.at(c) == 4)//直线
        {
            p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//反锯齿
            p.drawLine(_line.at(i4).topLeft(),_line.at(i4).bottomRight());
            i4++;
        }
        else if(_shape.at(c) == 5)//文本
        {
            p.setRenderHints(QPainter::TextAntialiasing);//反锯齿
            p.drawText(_tpoint.at(i5),_text.at(i5));
            i5++;
        }
        else if(_shape.at(c) == 6)//圆角矩形
        {
            p.setRenderHints(QPainter::TextAntialiasing);//反锯齿
            p.drawRoundedRect(_roundrects.at(i6++),20,20);
        }
    }
    p.end();
    p.begin(this);//将当前窗体作为画布
    p.drawPixmap(0,0, pix);//将pixmap画到窗体
}

void MyPaint::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)//当鼠标左键按下
    {
        if(_drawType == 1)//线条(铅笔)
        {
            _lpress = true;//左键按下标志
            QVector<QPoint> line;//鼠标按下，新的线条开始
            _lines.append(line);//将新线条添加到线条集合
            QVector<QPoint>& lastLine = _lines.last();//拿到新线条
            lastLine.append(e->pos());//记录鼠标的坐标(新线条的开始坐标)
            _shape.append(1);
        }
        else if(_drawType == 2)//矩形
        {
            _lpress = true;//左键按下标志
            if(!_drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，矩形开始
                _rects.append(rect);//将新矩形添加到矩形集合
                QRect& lastRect = _rects.last();//拿到新矩形
                lastRect.setTopLeft(e->pos());//记录鼠标的坐标(新矩形的左上角坐标)
                 _shape.append(2);
            }
            else if(_rects.last().contains(e->pos()))//拖拽模式、如果在矩形内按下
            {
                _begin = e->pos();//记录拖拽开始的坐标位置,方便计算位移

            }

        }
        else if(_drawType == 3)//椭圆
        {
            _lpress = true;//左键按下标志
            if(!_drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，椭圆开始
                _ellipse.append(rect);//将新椭圆添加到椭圆集合
                QRect& lastEllipse = _ellipse.last();//拿到新椭圆
                lastEllipse.setTopLeft(e->pos());//记录鼠标的坐标(新椭圆的左上角坐标)
                 _shape.append(3);
            }
            else if(_ellipse.last().contains(e->pos()))//如果在椭圆内按下
            {
                _begin = e->pos();//记录拖拽开始的坐标位置

            }
        }
        else if(_drawType == 4)//直线
        {
            _lpress = true;//左键按下标志
            QRect rect;//鼠标按下，直线一端开始
            _line.append(rect);//将新直线添加到直线集合
            QRect& lastLine = _line.last();//拿到新直线
            lastLine.setTopLeft(e->pos());//记录鼠标的坐标(新直线开始一端坐标)
            _shape.append(4);
        }
        else if(_drawType == 5)//文字
        {
            update();//触发窗体重绘
            QPoint p;//鼠标按下，文字开始
            _tpoint.append(p);//将文字坐标添加到文字位置集合
            QPoint& lastp = _tpoint.last();//拿到新文字
            lastp = e->pos();//记录鼠标的坐标
            _tEdit->setGeometry(lastp.x()-6,lastp.y()-17,90,90);//设置文本框的位置、大小

            _tEdit->show();//显示文本输入框
            _text.append("");//添加文本到文本集合
            _tEdit->clear();//因为全局只有一个，所以使用之前要清空
            _shape.append(5);
        }
        else if(_drawType == 6)//圆角矩形
        {
            _lpress = true;//左键按下标志
            if(!_drag)//非拖拽模式
            {
                QRect rect;//鼠标按下，矩形开始
                _roundrects.append(rect);//将新圆角矩形添加到圆角矩形集合
                QRect& lastRoundRect = _roundrects.last();//拿到新矩形
                lastRoundRect.setTopLeft(e->pos());//记录鼠标的坐标(新矩形的左上角坐标)
                 _shape.append(6);
            }
            else if(_roundrects.last().contains(e->pos()))//拖拽模式、如果在矩形内按下
            {
                _begin = e->pos();//记录拖拽开始的坐标位置,方便计算位移

            }
        }

    }
}

void MyPaint::AddTexts()//当输入框文本改变时调用
{
    QString& last = _text.last();//拿到最后一个文本
    last = _tEdit->toPlainText();//将输入框文本作为文本
}

void MyPaint::mouseMoveEvent(QMouseEvent *e)
{
    if(_drag &&
       (
           (_drawType == 2 && _rects.last().contains(e->pos()))
        || (_drawType == 3 && _ellipse.last().contains(e->pos()))
        || (_drawType == 6 && _roundrects.last().contains(e->pos()))
        )
       )
    {
        setCursor(Qt::SizeAllCursor);//拖拽模式下，并且在拖拽图形中，将光标形状改为十字
    }
    else
    {
        setCursor(Qt::ArrowCursor);//恢复原始光标形状
        _drag = 0;
    }

    if(_lpress)
    {
        px = QString::number(e->pos().x());
        py = QString::number(e->pos().y());

        pixLabel->setText(px+"X"+py);
        if(_drawType == 1)//铅笔画线
        {
            if(_lines.size()<=0) return;//线条集合为空，不画线
            QVector<QPoint>& lastLine = _lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录鼠标的坐标(线条的轨迹)
            pstatusLabel->setText("使用铅笔画线");//更新状态栏
            update();//触发窗体重绘
        }
        else if(_drawType == 2)
        {
            if(_drag == 0)//非拖拽
            {
                QRect& lastRect = _rects.last();//拿到新矩形
                lastRect.setBottomRight(e->pos());//更新矩形的右下角坐标
                pstatusLabel->setText("画矩形");//更新状态栏
            }
            else//拖拽模式
            {
                QRect& lastRect = _rects.last();//拿到最后添加的矩形
                if(lastRect.contains(e->pos()))//在矩形的内部
                {
                    int dx = e->pos().x()-_begin.x();//横向移动x
                    int dy = e->pos().y()-_begin.y();//纵向移动y
                    lastRect = lastRect.adjusted(dx,dy,dx,dy);//更新矩形的位置
                    _begin = e->pos();//刷新拖拽点起始坐标
                }
                pstatusLabel->setText("拖动矩形");//更新状态栏

            }
            update();//触发窗体重绘

        }
        else if(_drawType == 3)
        {
            if(_drag == 0)//非拖拽
            {
                QRect& lastEllipse = _ellipse.last();//拿到新椭圆
                lastEllipse.setBottomRight(e->pos());//更新椭圆的右下角坐标
                pstatusLabel->setText("画椭圆");//更新状态栏

            }
            else//拖拽
            {
                QRect& lastEllipse = _ellipse.last();//拿到最后添加的椭圆
                if(lastEllipse.contains(e->pos()))//在椭圆内部
                {
                    int dx = e->pos().x()-_begin.x();//横向移动x
                    int dy = e->pos().y()-_begin.y();//纵向移动y
                    lastEllipse = lastEllipse.adjusted(dx,dy,dx,dy);//更新椭圆的位置
                    _begin = e->pos();//刷新拖拽点起始坐标
                }
                pstatusLabel->setText("拖动椭圆");//更新状态栏

            }
            update();//触发窗体重绘
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//拿到新直线
            lastLine.setBottomRight(e->pos());//更新直线另一端
            pstatusLabel->setText("画直线");//更新状态栏
            update();//触发窗体重绘
        }
        else if(_drawType == 6)
        {
            if(_drag == 0)//非拖拽
            {
                QRect& lastRoundRect = _roundrects.last();//拿到新圆角矩形
                lastRoundRect.setBottomRight(e->pos());//更新圆角矩形的右下角坐标
                pstatusLabel->setText("画圆角矩形");//更新状态栏
            }
            else//拖拽模式
            {
                QRect& lastRoundRect = _roundrects.last();//拿到最后添加的矩形
                if(lastRoundRect.contains(e->pos()))//在矩形的内部
                {
                    int dx = e->pos().x()-_begin.x();//横向移动x
                    int dy = e->pos().y()-_begin.y();//纵向移动y
                    lastRoundRect = lastRoundRect.adjusted(dx,dy,dx,dy);//更新矩形的位置
                    _begin = e->pos();//刷新拖拽点起始坐标
                }
                pstatusLabel->setText("拖动圆角矩形");//更新状态栏

            }
            update();//触发窗体重绘
        }
    }
    else
    {
        px = QString::number(e->pos().x());
        py = QString::number(e->pos().y());

        pixLabel->setText(px+"X"+py);
        update();
        //cout << e->pos().x() << "," << e->pos().y() << endl;
    }

}

void MyPaint::mouseReleaseEvent(QMouseEvent *e)
{
    if(_lpress)
    {
        if(_drawType == 1)
        {
            QVector<QPoint>& lastLine = _lines.last();//最后添加的线条，就是最新画的
            lastLine.append(e->pos());//记录线条的结束坐标
            _lpress = false;//标志左键释放
        }
        else if(_drawType == 2)
        {
            QRect& lastRect = _rects.last();//拿到新矩形
            if(!_drag)
            {
                lastRect.setBottomRight(e->pos());//不是拖拽时，更新矩形的右下角坐标)
                //刚画完矩形，将光标设置到新矩形的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastRect.width()/2,this->cursor().pos().y()-lastRect.height()/2);
                _drag = 1;//进入拖拽模式

            }
            _lpress = false;//标志左键释放

        }
        else if(_drawType == 3)
        {
            QRect& lastEllipse = _ellipse.last();//拿到新椭圆
            if(!_drag)
            {
                lastEllipse.setBottomRight(e->pos());//不是拖拽时，更新椭圆的右下角坐标)
                //刚画完椭圆，将光标设置到新椭圆的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastEllipse.width()/2,this->cursor().pos().y()-lastEllipse.height()/2);
                _drag = 1;//进入拖拽模式

            }
            _lpress = false;
        }
        else if(_drawType == 4)
        {
            QRect& lastLine = _line.last();//拿到新直线
            lastLine.setBottomRight(e->pos());//更新直线的右下角坐标
            _lpress = false;

        }
        else if(_drawType == 6)
        {
            QRect& lastRoundRect = _roundrects.last();//拿到新矩形
            if(!_drag)
            {
                lastRoundRect.setBottomRight(e->pos());//不是拖拽时，更新矩形的右下角坐标)
                //刚画完矩形，将光标设置到新矩形的中心位置，并进入拖拽模式
                this->cursor().setPos(this->cursor().pos().x()-lastRoundRect.width()/2,this->cursor().pos().y()-lastRoundRect.height()/2);
                _drag = 1;//进入拖拽模式

            }
            _lpress = false;//标志左键释放
        }
    }
}

void MyPaint::Lines()
{
    _drawType = 1;//铅笔
    _tEdit->hide();//文本框隐藏
    pstatusLabel->setText("铅笔");
    update();
}

void MyPaint::Line()
{
    _drawType = 4;//直线
    _tEdit->hide();
    pstatusLabel->setText("直线");
    update();
}

void MyPaint::Rects()
{
    _drawType = 2;//矩形
    _tEdit->hide();
    pstatusLabel->setText("矩形");
    update();

}

void MyPaint::RoundRects()
{
    _drawType = 6;//圆角矩形
    _tEdit->hide();
    pstatusLabel->setText("圆角矩形");
    update();
}

void MyPaint::Ellipses()
{
    _drawType = 3;//椭圆
    _tEdit->hide();
    pstatusLabel->setText("椭圆");
    update();
}

void MyPaint::Texts()
{
    _drawType = 5;//文字
}

//改变画笔颜色
void MyPaint::RedBrush()
{
    _brush = Qt::red;
}

void MyPaint::BlueBrush()
{
    _brush = Qt::blue;
}

void MyPaint::BlackBrush()
{
    _brush = Qt::black;
}

void MyPaint::SavePic()
{
    //获取时间戳作为文件名
    time_t currentTime = time(NULL);
    tm* p = localtime(&currentTime);
    char fname[100] = {0};

    sprintf(fname,"p%d%02d%02d%02d%02d%02d.jpg",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), fname, "Image (*.jpg *.png *.bmp)");

    if (fileName.length() > 0)
    {
        _tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(0,60,size().width(),size().height())).save(fileName);//不包含工具栏
    }
}

void MyPaint::SaveAsPic()
{
    //获取时间戳作为文件名
    time_t currentTime = time(NULL);
    tm* p = localtime(&currentTime);
    char fname[100] = {0};

    sprintf(fname,"p%d%02d%02d%02d%02d%02d.jpg",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

    //弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"), fname, "Image (*.jpg *.png *.bmp)");

    if (fileName.length() > 0)
    {
        _tEdit->hide();//防止文本输入框显示时，将文本框保存到图片
        QPixmap pixmap(size());//新建窗体大小的pixmap
        QPainter painter(&pixmap);//将pixmap作为画布
        painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
        this->render(&painter);//将窗体渲染到painter，再由painter画到画布
        pixmap.copy(QRect(0,60,size().width(),size().height())).save(fileName);//不包含工具栏
    }
}

void MyPaint::OpenPic()
{
    //弹出文件打开对话框
    QString picPath = QFileDialog::getOpenFileName(this,tr("打开"),"","Image Files(*.jpg *.png)");
    if(!picPath.isEmpty())//用户选择了文件
    {
        QPixmap pix;
        pix.load(picPath);//加载图片
        QPainter p(&_pixmap);
        p.drawPixmap(0,30,pix);//添加工具栏的空间
        _openflag = 1;//设置文件打开标志
        update();//触发窗体重绘，将图片画到窗体
    }
}

void MyPaint::contextMenuEvent(QContextMenuEvent *)  //右键菜单事件
{
    _Rmenu->exec(cursor().pos());//在光标位置弹出菜单
    pstatusLabel->setText("右键菜单");//更新状态栏
    update();//刷新界面
}

void MyPaint::keyPressEvent(QKeyEvent *e) //按键事件
{
     //Ctrl+Z撤销
     if (e->key() == Qt::Key_Z && e->modifiers() == Qt::ControlModifier)
     {
         if(_shape.size()>0)
         {
             switch(_shape.last())
             {
                 case 1: _lines.pop_back();
                         break;
                 case 2: _rects.pop_back();
                         break;
                 case 3: _ellipse.pop_back();
                         break;
                 case 4: _line.pop_back();
                         break;
                 case 6:_roundrects.pop_back();
                         break;
             }
             _shape.pop_back();
             _drag = 0;//设置为非拖拽模式
             pstatusLabel->setText("撤销了一个操作");//更新状态栏
             update();
         }
     }
     //Ctrl+S保存
     else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)//保存
     {
        SavePic();
     }
}

void MyPaint::GetHelp(){
    QDialog *helpDialog = new QDialog();
    helpDialog->resize(800,600);
    helpDialog->setWindowIcon(QIcon(":/png/images/help.png"));
    helpDialog->setWindowTitle("帮助");

    QLabel *helpLabel = new QLabel(helpDialog);
    helpLabel->setText("欢迎使用本软件！\n\n您是否需要以下帮助？\n\n通用快捷键：\n\n"
        "保存：Ctrl + S\n\n另存为：Ctrl + Alt + S\n\n撤销：Ctrl + Z\n\n打开：Ctrl + O\n\n"
        "多人协作绘图：Ctrl + E\n\n帮助：Ctrl + H\n\n"
        "图像快捷键：\n\n矩形：Ctrl + R\n\n椭圆：Ctrl + E\n\n文字：Ctrl + T\n\n直线：Ctrl + L"
        "\n\n铅笔：Ctrl + P");
    helpLabel->setMargin(10);

    helpDialog->exec();
}

void MyPaint::GetAbout(){
    QDialog *aboutDialog = new QDialog();
    aboutDialog->resize(800,600);
    aboutDialog->setWindowIcon(QIcon(":/png/images/info.png"));
    aboutDialog->setWindowTitle("MyPaint 1.0");

    QLabel *aboutLabel = new QLabel(aboutDialog);
    aboutLabel->setText("MyPaint v1.0\n\n作者：tihongsheng@foxmail.com\n\n运行平台：Windows（请保证显示器分辨率至少为1280X720，否则软件的窗口将超出屏幕hud边界）"
                        "\n\n使用方法：\n1. 使用打开功能加载图片，或者直接在空白画布上绘图\n"
                        "2. 选择工具栏中的图形进行绘图。松开鼠标后，指针会自动到图形中心方便拖动\n"
                        "3. 保存\n\n"
                        "已实现的功能：\n"
                        "2)讨论版支持插入文字和图形标记，图形标记包括：直线、圆、椭圆和矩形。\n"
                        "3)用户可以增加、删除、选择、移动以上的文字和图形标记，也可以改变标记的属性，如颜色、宽度、字体、大小等。\n\n"
                        "注意事项：\n"
                        "已知搜狗输入法的部分功能可能导致软件崩溃\n\n"
                        "部分代码参考自 Genven_Liang、Jack_123 ，在此表示感谢。");
    aboutLabel->setMargin(10);

    aboutDialog->exec();
}

void MyPaint::ErrorFunction(){
    QDialog *errorDialog = new QDialog();
    errorDialog->resize(400,300);
    errorDialog->setWindowIcon(QIcon(":/png/images/warn.png"));
    errorDialog->setWindowTitle("ERROR!");

    QLabel *errorLabel = new QLabel(errorDialog);
    errorLabel->setText("ERROR!");
    errorLabel->setMargin(20);

    errorDialog->exec();
}

void MyPaint::PointCursor(){
    _drawType = 0;//矩形
    _tEdit->hide();
    pstatusLabel->setText("就绪");
    update();
}

void MyPaint::UndoPic(){
    if(_shape.size()>0)
    {
        switch(_shape.last())
        {
            case 1: _lines.pop_back();
                    break;
            case 2: _rects.pop_back();
                    break;
            case 3: _ellipse.pop_back();
                    break;
            case 4: _line.pop_back();
                    break;
            case 6:_roundrects.pop_back();
                break;
        }
        _shape.pop_back();
        _drag = 0;//设置为非拖拽模式
        pstatusLabel->setText("撤销了一个操作");
        update();
    }
}

void MyPaint::GetPicFromClipboard(){
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *rawData = clipboard->mimeData();

    if(rawData->hasImage()){
        QPixmap pix = qvariant_cast<QPixmap>(rawData->imageData());
        QPainter p(&_pixmap);
        p.drawPixmap(0,30,pix);//添加工具栏的空间
        _openflag = 1;//设置文件打开标志
        update();//触发窗体重绘，将图片画到窗体
    }else {
        ErrorFunction();
    }
}

void MyPaint::SetPicToClipboard(){
    QClipboard *clipboard = QApplication::clipboard();
    //QMimeData mimeData;

    QPixmap pixmap(size());//新建窗体大小的pixmap
    QPainter painter(&pixmap);//将pixmap作为画布
    painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
    this->render(&painter);//将窗体渲染到painter，再由painter画到画布
    pixmap.copy(QRect(0,60,size().width(),size().height()-30));//不包含工具栏

    if(!pixmap){
        ErrorFunction();
    }

    //mimeData.setImageData(pixmap);
    clipboard->setPixmap(pixmap);
}

void MyPaint::EmailToAuthor(){
    QDesktopServices::openUrl(QUrl("mailto:tihongsheng@foxmail.com"));
}

void MyPaint::VisitBlog(){
    QDesktopServices::openUrl(QUrl("https://bluebonnet27.github.io"));
}

void MyPaint::VisitGithub(){
    QDesktopServices::openUrl(QUrl("https://github.com/bluebonnet27/copaint"));
}

void MyPaint::ClearAll(){
    while(_shape.size()>0)
    {
        switch(_shape.last())
        {
            case 1: _lines.pop_back();
                    break;
            case 2: _rects.pop_back();
                    break;
            case 3: _ellipse.pop_back();
                    break;
            case 4: _line.pop_back();
                    break;
            case 6:_roundrects.pop_back();
                    break;
        }
        _shape.pop_back();
    }
    _drag = 0;//设置为非拖拽模式
    _drawType = 0;//不画任何图形
    pstatusLabel->setText("所有图形都被清除了");//更新状态栏提示
    update();
}
