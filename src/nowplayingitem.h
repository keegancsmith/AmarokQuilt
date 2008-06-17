/**************************** -*- mode: c++ -*- ****************************
 *                                                                         *
 *   file            : nowplayingitem.h                                    *
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

#ifndef NOWPLAYING_H
#define NOWPLAYING_H

#include <QString>
#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#define SIZEBIG 192

class NowPlayingItem : public QObject, public QGraphicsItemGroup {
    Q_OBJECT;

public:
    NowPlayingItem();

protected:
    void timerEvent(QTimerEvent *);

private:
    void updateNowPlaying();
    void updateText();
    void updateArtwork();
    QString amarokPlayerDCOP(const QString &) const;

    QGraphicsPixmapItem m_textitem;
    QGraphicsPixmapItem m_artworkitem;
    QPixmap m_artwork;
    QPixmap m_text;

    QString m_artworkstr;
    QString m_textstr;
};


#endif
