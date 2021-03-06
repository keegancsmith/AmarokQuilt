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
#include <QDesktopWidget>
#include <QFontDialog>
#include <QSettings>
#include <cstdlib>

#include "amarokquilt.h"

void configDialog() {
    // Config dialog for nowplaying font
    QSettings settings("AmarokQuilt");
    QFont font = QFont("Verdana", 26, QFont::Bold);
    font = settings.value("nowplayingfont", font).value<QFont>();
    
    bool ok;
    font = QFontDialog::getFont(&ok, font, 0, "Now Playing Font");
    
    if (ok)
        settings.setValue("nowplayingfont", font);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    std::srand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QString mode = "-demo"; // default mode
    WId window = 0; // default window id

    if (app.argc() > 1)
        mode = app.argv()[1];

    for (int i = 1; i < argc; i++) {
        if (QString(argv[i]) == "-setup") {
            configDialog();
            return 0;
        }
    }

    if (mode == "-root")
        window = QApplication::desktop()->winId();
    else if (app.argc() > 2 && mode == "-window-id")
        window = QString(app.argv()[2]).toULong();

    AmarokQuilt amarokQuilt(window);
    amarokQuilt.show();

    return app.exec();
}
