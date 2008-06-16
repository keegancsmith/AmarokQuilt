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
