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
#include <QThread>
#include <cstdlib>

#include "amarokquilt.h"

AmarokQuilt::AmarokQuilt(WId window) {
    QRect size;
    
    if (window) {
        // Use an existing window
        create(window, false, true);
        size = geometry();
    } else {
        // Get the dimensions of the screen
        size = QApplication::desktop()->screenGeometry();
    }

    int width     = size.width();
    int height    = size.height();
    int item_size = std::min(width / 12, height / 8);
    if (item_size < 40)
        item_size = 40;
    if (item_size > 200)
        item_size = 200;
    qreal wdelta = (width % item_size) / 2;
    qreal hdelta = (height % item_size) / 2;

    // Setup a scene the size of the screen with a black background
    m_scene.setSceneRect(0, 0, width, height);
    m_scene.setBackgroundBrush(QBrush(Qt::black));
    m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);

    // Add each cd artwork cell to the scene
    for (int r = 0; r <= width; r += item_size) {
        for (int c = 0; c <= height; c+= item_size) {
            AlbumArtworkItem *cover = new AlbumArtworkItem(item_size);
            m_artworks << cover;
            cover->setPos(r - hdelta, c - wdelta);
            m_scene.addItem(cover);
        }
    }
    
    // Add the item for displaying the current track name and artwork.
    m_nowPlaying = new NowPlayingItem(item_size * 1.92);
    m_nowPlaying->setPos(item_size * 0.9, item_size * 0.9);
    m_nowPlaying->setZValue(9999);
    m_scene.addItem(m_nowPlaying);

    if (!window)
        setWindowState(windowState() | Qt::WindowFullScreen);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle("AmarokQuilt");
    setScene(&m_scene);

    // For some reason app resized itself. Fixes size
    setFixedWidth(width);
    setFixedHeight(height);

    // Start loading album images in a seperate thread. Check if this is a
    // memory leak.
    ArtworkLoaderThread *loader = new ArtworkLoaderThread(this);
    loader->start();

    // Switch a cd cover every 1.5 seconds
    startTimer(1500);
}

void AmarokQuilt::timerEvent(QTimerEvent *) {
    int index = std::rand() % m_artworks.size();
    m_artworks[index]->updateArtwork();
}

void AmarokQuilt::addArtworks() {
    // Shuffle artworks list
    for (int n = 0; n < m_artworks.size(); n++)
        m_artworks.swap(std::rand() % m_artworks.size(),
                        std::rand() % m_artworks.size());

    // Load image
    AlbumArtworkItem *cover;
    foreach(cover, m_artworks)
        cover->updateArtwork();

    // Switch a cd cover every 1.5 seconds
    startTimer(1500);
}

void ArtworkLoaderThread::run() {
    m_quilt->addArtworks();
}
