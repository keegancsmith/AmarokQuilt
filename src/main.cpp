#include <QApplication>
#include <QTime>

#include "amarokquilt.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    AmarokQuilt amarokQuilt;
    amarokQuilt.show();

    return app.exec();
}
