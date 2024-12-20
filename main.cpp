#include "algorithms.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    int n=5;
    QScopedPointer<QWidget> widget(new Algorithms(nullptr,n,screenWidth,screenHeight));
    widget->setGeometry(0, 0, screenWidth, screenHeight);
    widget->show();
    return a.exec();
}
