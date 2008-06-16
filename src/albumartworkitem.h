#ifndef ALBUMARTWORK_H
#define ALBUMARTWORK_H

#include <QGraphicsPixmapItem>
#include <QTimeLine>

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

    QPixmap m_artwork;
    QPixmap m_artworktmp;
    QTimeLine m_timeline;
    bool m_updatingArtwork;
    bool m_updateArtwork;
};

#endif
