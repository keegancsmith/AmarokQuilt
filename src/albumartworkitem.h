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

class AlbumArtworkItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT;

public:
    AlbumArtworkItem(int size = 100);
    void updateArtwork();

public slots:
    void updateAnimation(int);
    void finishedAnimation();

private:
    void _updateArtwork();
    void _switchArtwork();

    QPixmap m_artwork;       // Currently displayed artwork
    QPixmap m_artworktmp;    // Buffer of next artwork to display.
    QTimeLine m_timeline;    // Timeline used in animation
    bool m_updatingArtwork;  // True if currently animating
    bool m_updateArtwork;    // True if artwork old artwork is still displayed
    int m_size;              // Width and height of item

    // Following variables are stored to keep track of what is being
    // displayed. This is used to reduce images being shown more than once.
    static QMap<QString, int> m_usedArtworks;
    QString m_artworkFile;
    QString m_artworkFileTmp;
};

#endif
