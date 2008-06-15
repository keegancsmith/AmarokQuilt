#ifndef NOWPLAYING_H
#define NOWPLAYING_H

#include <QString>
#include <QObject>
#include <QPixmap>
#include <QGraphicsPixmapItem>

#define SIZE 75
#define SIZEBIG 175

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
