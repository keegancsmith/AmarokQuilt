/**************************** -*- mode: c++ -*- ****************************
 *                                                                         *
 *   file            : albumartworkitem.cpp                                *
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

#include "albumartworkitem.h"

#include <QDir>
#include <QDebug>
#include <cstdlib>

QMap<QString, int> AlbumArtworkItem::m_usedArtworks;

AlbumArtworkItem::AlbumArtworkItem() {
    m_artwork = 0;
    m_updatingArtwork = false;
    _updateArtwork();
    _switchArtwork();

    m_timeline.setFrameRange(0, 100);
    m_timeline.setDuration(2100);
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
            _switchArtwork();
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
    
    // Try five times to find non-conflicting images
    for (int i = 0; i < 5; i++) {
        m_artworkFileTmp = coverList[std::rand() % coverList.size()];
        if (AlbumArtworkItem::m_usedArtworks[m_artworkFileTmp] == 0)
            break;
    }
    AlbumArtworkItem::m_usedArtworks[m_artworkFileTmp]++;
    
    qDebug() << "Loading " << m_artworkFileTmp
             << AlbumArtworkItem::m_usedArtworks[m_artworkFileTmp];
    QPixmap pixmap(dir.absoluteFilePath(m_artworkFileTmp));
    m_artworktmp = pixmap.scaled(QSize(SIZE,SIZE),
                                 Qt::KeepAspectRatioByExpanding,
                                 Qt::SmoothTransformation)
        .copy(0, 0, SIZE, SIZE);
}

void AlbumArtworkItem::_switchArtwork() {
    if (!m_artworkFile.isNull())
        AlbumArtworkItem::m_usedArtworks[m_artworkFile]--;    
    m_artworkFile = m_artworkFileTmp;
    m_artwork = m_artworktmp;
    m_artworktmp = QPixmap();
    setPixmap(m_artwork);
}
