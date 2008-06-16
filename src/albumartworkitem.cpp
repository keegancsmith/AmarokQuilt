#include "albumartworkitem.h"

#include <QDir>
#include <QDebug>

AlbumArtworkItem::AlbumArtworkItem() {
    m_artwork = 0;
    m_updatingArtwork = false;
    _updateArtwork();
    
    m_artwork = m_artworktmp;
    m_artworktmp = QPixmap();
    setPixmap(m_artwork);

    m_timeline.setFrameRange(0, 100);
    m_timeline.setDuration(2500);
    connect(&m_timeline, SIGNAL(frameChanged(int)), SLOT(updateAnimation(int)));
    connect(&m_timeline, SIGNAL(finished()), SLOT(finishedAnimation()));
}    

void AlbumArtworkItem::updateAnimation(int i) {
    qreal yscale;
    if (i >= 50) {
        // Scale in new image
        yscale = (i-50)/50.0;

        // Load new image if necessary
        if (m_updateArtwork) {
            m_artwork = m_artworktmp;
            m_artworktmp = QPixmap();
            setPixmap(m_artwork);
            m_updateArtwork = false;
        }
    } else {
        // Scale out old image
        yscale = (50-i)/50.0;
    }

    resetTransform();
    translate(0, (SIZE - SIZE * yscale) / 2.0);
    scale(1, yscale);
}

void AlbumArtworkItem::finishedAnimation() {
    resetTransform();
}

void AlbumArtworkItem::updateArtwork() {
    if (m_updatingArtwork)
        return;

    _updateArtwork();
    m_updateArtwork = true;
    m_updatingArtwork = true;
    m_timeline.start();
}

void AlbumArtworkItem::_updateArtwork() {
    QDir dir(QDir::homePath() + "/.kde/share/apps/amarok/albumcovers/large/");
    QStringList coverList = dir.entryList(QDir::Files);
    //coverList = coverList.filter(QRegExp("^.{35}$"));
    int imageIndex = qrand() % coverList.size();
    
    qDebug() << "Loading " << dir.absoluteFilePath(coverList[imageIndex]);
    QPixmap pixmap(dir.absoluteFilePath(coverList[imageIndex]));
    m_artworktmp = pixmap.scaled(QSize(SIZE,SIZE),
                                 Qt::KeepAspectRatioByExpanding,
                                 Qt::SmoothTransformation)
        .copy(0, 0, SIZE, SIZE);
}
