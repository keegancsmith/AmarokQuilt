/**************************** -*- mode: c++ -*- ****************************
 *                                                                         *
 *   file            : amarokquilt.cpp                                     *
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
#include <QDesktopWidget>
#include <cstdlib>

#include "amarokquilt.h"

AmarokQuilt::AmarokQuilt() {
    QRect screenSize = QApplication::desktop()->screenGeometry();
    int width = screenSize.width();
    int height = screenSize.height();
    qreal wdelta = (width % SIZE) / 2;
    qreal hdelta = (height % SIZE) / 2;

    m_scene.setSceneRect(0, 0, width, height);
    m_scene.setBackgroundBrush(QBrush(Qt::black));
    m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    
    for (int r = 0; r <= width; r+=SIZE) {
        for (int c = 0; c <= height; c+= SIZE) {
            AlbumArtworkItem *cover = new AlbumArtworkItem();
            m_artworks << cover;
            cover->setPos(r - hdelta, c - wdelta);
            m_scene.addItem(cover);
        }
    }

    m_nowPlaying.setPos(90, 90);
    m_nowPlaying.setZValue(9999);
    m_scene.addItem(&m_nowPlaying);

    setWindowState(windowState() | Qt::WindowFullScreen);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle("AmarokQuilt");
    setScene(&m_scene);

    startTimer(1500);
}

void AmarokQuilt::timerEvent(QTimerEvent *) {
    int index = std::rand() % m_artworks.size();
    m_artworks[index]->updateArtwork();
}
