#include <QApplication>
#include "mypaint.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    MyPaint *w = new MyPaint();
    w->setWindowIcon(QIcon(":/png/images/1.png"));
    w->setWindowTitle("画图");
    w->show();
    return app.exec();
}
