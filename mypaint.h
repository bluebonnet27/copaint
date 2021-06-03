#ifndef MYPAINT_H
#define MYPAINT_H

#include <string>
#include <time.h>

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

#include <QMessageBox>
#include <QLabel>
#include <QScrollArea>
#include <QStatusBar>
#include <QMenuBar>
#include <QString>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

using namespace std;

class MyPaint : public QMainWindow
{
    Q_OBJECT
public:
    explicit MyPaint(QWidget *parent = 0);
protected:
    //重写窗体重绘事件，来自QWidget
    void paintEvent(QPaintEvent *);
    //重写鼠标按下事件，来自QWidget，以下三个鼠标事件均来自同一个库
    void mousePressEvent(QMouseEvent *);
    //重写鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *);
    //重写鼠标移动事件
    void mouseMoveEvent(QMouseEvent *);
    //重写菜单事件，来自QWidget
    void contextMenuEvent(QContextMenuEvent *);
     //重写按键事件，来自QWidget
    void keyPressEvent(QKeyEvent *e);
public:
private:
    int _lpress;//左键按下标志
    int _drag;//拖拽标志
    int _drawType;//描绘类型
    int _shapes;//
    QMenu *_Rmenu;//右键菜单
    int _openflag;//打开图片
    QPixmap _pixmap;//画布图片
    QTextEdit *_tEdit;//文本输入框
    QBrush _brush;
    QLabel *pstatusLabel = new QLabel(QStringLiteral("就绪"));
    QLabel *pixLabel = new QLabel(QStringLiteral("1280x720"));
    QString px;
    QString py;
public:
    QVector<QVector<QPoint> > _lines;//线条集合(一条线条可包含多个线段)
    QVector<QRect> _rects;//矩形集合
    QVector<QRect> _ellipse;//椭圆集合
    QVector<QRect>  _line;//直线集合
    QVector<QString>  _text;//文字集合
    QVector<QPoint>  _tpoint;//文字位置集合
    QVector<int>  _shape;//图形类型集合，用于撤回功能
    QPoint _begin;//鼠标按下坐标、用于最后一个图形移动
signals:

public slots:
    void Lines();//铅笔画线
    void SavePic();//保存图片
    void SaveAsPic();//图片另存为
    void UndoPic();//撤销操作
    void PointCursor();//恢复鼠标指针
    void Rects();//画矩形
    void Ellipses();//画椭圆
    void Line();//画直线
    void OpenPic();//打开图片
    void Texts();//文字
    void AddTexts();//添加文字
    void RedBrush();
    void BlueBrush();
    void BlackBrush();
    void GetPicFromClipboard();//从剪切板获取图片
    void SetPicToClipboard();//把图片复制到剪切板里
    void GetHelp();//获取帮助
    void GetAbout();//获取软件信息
    void ErrorFunction();//未开发的功能的提示
};

#endif // MYPAINT_H
