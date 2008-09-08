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
#include <QImage>
#include <cstdlib>

#define FRAME_RANGE 100

QMap<QString, int> AlbumArtworkItem::m_usedArtworks;

AlbumArtworkItem::AlbumArtworkItem(int size) {
    m_size = size;
    m_updatingArtwork = false;   // Not animating
    _updateArtwork();            // Load an album cover
    _switchArtwork();            // Display the loaded album cover

    // Setup animation to run for 1.5 seconds.
    m_timeline.setFrameRange(0, FRAME_RANGE);
    m_timeline.setDuration(1500);
    m_timeline.setCurveShape(QTimeLine::LinearCurve);
    connect(&m_timeline, SIGNAL(frameChanged(int)), SLOT(updateAnimation(int)));
    connect(&m_timeline, SIGNAL(finished()), SLOT(finishedAnimation()));
}

void AlbumArtworkItem::updateAnimation(int i) {
    qreal yscale;
    qreal midpoint = FRAME_RANGE / 2;
    if (i >= midpoint) {
        // Scale in new image
        yscale = (i-midpoint)/midpoint;

        // Display new image if necessary
        if (m_updateArtwork) {
            _switchArtwork();
            m_updateArtwork = false;
        }
    } else {
        // Scale out old image
        yscale = (midpoint-i)/midpoint;
    }

    resetTransform();
    translate(0, (m_size - m_size * yscale) / 2.0);
    scale(1, yscale);
}

void AlbumArtworkItem::finishedAnimation() {
    resetTransform();
}

void AlbumArtworkItem::updateArtwork() {
    // Already animating, so ignore call
    if (m_updatingArtwork)
        return;
    
    // Load next artwork to be displayed, then animate it in.
    _updateArtwork();
    m_updateArtwork = true;
    m_updatingArtwork = true;
    m_timeline.start();
}

void AlbumArtworkItem::_updateArtwork() {
    // Get a list of cached amarok albums
    QDir dir(QDir::homePath() + "/.kde/share/apps/amarok/albumcovers/large/");
    QStringList coverList = dir.entryList(QDir::Files);
    
    // Try five times to find non-conflicting images. This reduces the chances
    // of displaying an image twice at the same time greatly.
    for (int i = 0; i < 5; i++) {
        m_artworkFileTmp = coverList[std::rand() % coverList.size()];
        if (AlbumArtworkItem::m_usedArtworks[m_artworkFileTmp] == 0)
            break;
    }
    AlbumArtworkItem::m_usedArtworks[m_artworkFileTmp]++;
    
    // Load selected cover into the temporary pixmap m_artworktmp
    qDebug() << "Loading " << m_artworkFileTmp
             << AlbumArtworkItem::m_usedArtworks[m_artworkFileTmp];
    QImage image(dir.absoluteFilePath(m_artworkFileTmp));
    image = image.scaled(QSize(m_size,m_size),
                         Qt::KeepAspectRatioByExpanding,
                         Qt::SmoothTransformation);
    m_artworktmp = QPixmap::fromImage(image.copy(0, 0, m_size, m_size));
}

void AlbumArtworkItem::_switchArtwork() {
    // Update global list of displayed artworks
    if (!m_artworkFile.isNull())
        AlbumArtworkItem::m_usedArtworks[m_artworkFile]--;

    // Set the current artwork to the tmp one, and remove the references to tmp.
    m_artworkFile = m_artworkFileTmp;
    m_artwork = m_artworktmp;
    m_artworktmp = QPixmap();
    setPixmap(m_artwork);
}
