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

#include "nowplayingitem.h"
#include "albumartworkitem.h"

class AmarokQuilt : public QGraphicsView {
    Q_OBJECT;

public:
    AmarokQuilt();
    void timerEvent(QTimerEvent *);

private:
    QGraphicsScene m_scene;
    QList<AlbumArtworkItem* > m_artworks;
    NowPlayingItem m_nowPlaying;
};

#endif
