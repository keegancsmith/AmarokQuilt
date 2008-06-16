
#include <QApplication>
#include <QDesktopWidget>

#include "amarokquilt.h"

AmarokQuilt::AmarokQuilt() {
    QRect screenSize = QApplication::desktop()->screenGeometry();
    int width = screenSize.width();
    int height = screenSize.height();
    qreal wdelta = (width % SIZE) / 2;
    qreal hdelta = (height % SIZE) / 2;

    m_scene.setSceneRect(0, 0, width, height);
    m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    
    for (int r = 0; r <= width; r+=SIZE) {
        for (int c = 0; c <= height; c+= SIZE) {
            AlbumArtworkItem *cover = new AlbumArtworkItem();
            m_artworks << cover;
            cover->setPos(r - hdelta, c - wdelta);
            m_scene.addItem(cover);
        }
    }

    m_nowPlaying.setPos(90, 90);
    m_nowPlaying.setZValue(9999);
    m_scene.addItem(&m_nowPlaying);

    setWindowState(windowState() | Qt::WindowFullScreen);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowTitle("AmarokQuilt");
    setScene(&m_scene);

    startTimer(1500);
}

void AmarokQuilt::timerEvent(QTimerEvent *) {
    int index = qrand() % m_artworks.size();
    m_artworks[index]->updateArtwork();
}
