/**************************** -*- mode: c++ -*- ****************************
 *                                                                         *
 *   file            : main.cpp                                            *
 *   date started    : 15 Jun 2008                                         *
 *   author          : Keegan Carruthers-Smith                             *
 *   email           : keegan.csmith@gmail.com                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QTime>
#include <cstdlib>

#include "amarokquilt.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    std::srand(QTime(0,0,0).secsTo(QTime::currentTime()));

    AmarokQuilt amarokQuilt;
    amarokQuilt.show();

    return app.exec();
}
