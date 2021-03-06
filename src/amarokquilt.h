/**************************** -*- mode: c++ -*- ****************************
 *                                                                         *
 *   file            : amarokquilt.h                                       *
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

#ifndef AMAROKQUILT_H
#define AMAROKQUILT_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QThread>

#include "nowplayingitem.h"
#include "albumartworkitem.h"

class AmarokQuilt : public QGraphicsView {
    Q_OBJECT;

public:
    AmarokQuilt(WId window = 0);
    void timerEvent(QTimerEvent *);
    void addArtworks();

private:
    QGraphicsScene m_scene;
    QList<AlbumArtworkItem* > m_artworks;
    NowPlayingItem *m_nowPlaying;
};

class ArtworkLoaderThread : public QThread {
public:
    ArtworkLoaderThread(AmarokQuilt *quilt) : m_quilt(quilt) {}
    void run();
private:
    AmarokQuilt *m_quilt;
};

#endif
