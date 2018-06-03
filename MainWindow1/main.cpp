#include "mainwindow.h"
#include <QApplication>

QString bgColor="#1F1F1F";
QString appStyle= QString("QToolButton {{border: 0px solid #0F0F0F; background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #7A7A7A, stop: 1 #0F0F0F); color: #EBEBEB}} QMenu QAbstractItemView  {{ border: 0px solid black; background-color: #7A7A7A; color: #EBEBEB; border-radius: 0; }}QMenu {{  font-size:10pt; selection-background-color: #ffaa00; selection-color: black; background-color: #7A7A7A; border-style: solid; border: 0px solid #EBEBEB; border-radius: 0; color: #EBEBEB; padding: 0px 0px 0px 0px; }} QMenu:on  {{padding-top: 0px; padding-left: 0px; background-color: #7A7A7A; selection-background-color: #ffaa00; color: #EBEBEB; border-radius: 0;}} QMenu:hover {{ border: 0px solid #ffa02f; }} QMenu::drop-down  {{ border-radius: 0px; background-color: #7A7A7A; color: #EBEBEB; }}");


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
