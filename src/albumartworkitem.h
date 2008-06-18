/**************************** -*- mode: c++ -*- ****************************
 *                                                                         *
 *   file            : albumartworkitem.h                                  *
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

#ifndef ALBUMARTWORK_H
#define ALBUMARTWORK_H

#include <QGraphicsPixmapItem>
#include <QTimeLine>
#include <QMap>

#define SIZE 100

class AlbumArtworkItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT;

public:
    AlbumArtworkItem();
    void updateArtwork();

public slots:
    void updateAnimation(int);
    void finishedAnimation();

private:
    void _updateArtwork();
    void _switchArtwork();

    QPixmap m_artwork;
    QPixmap m_artworktmp;
    QTimeLine m_timeline;
    bool m_updatingArtwork;
    bool m_updateArtwork;

    static QMap<QString, int> m_usedArtworks;
    QString m_artworkFile;
    QString m_artworkFileTmp;
};

#endif
