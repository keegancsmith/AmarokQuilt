/**************************** -*- mode: c++ -*- ****************************
 *                                                                         *
 *   file            : nowplayingitem.cpp                                  *
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

#include "nowplayingitem.h"

#include <QPainterPath>
#include <QPainter>
#include <QFont>
#include <QProcess>
#include <QTextCodec>

NowPlayingItem::NowPlayingItem(int size) {
    m_size = size;
    m_artworkitem.setParentItem(this);
    m_textitem.setParentItem(this);
    m_artworkitem.setPos(0,0);

    // Checks what's playing every second
    startTimer(1000);
}

void NowPlayingItem::timerEvent(QTimerEvent *) {
    updateNowPlaying();
}

void NowPlayingItem::updateNowPlaying() {
    if (amarokPlayerDCOP("isPlaying") != "true") {
        setVisible(false);
        return;
    }
    setVisible(true);
    
    updateText();
    updateArtwork();
}

void NowPlayingItem::updateText() {
    QString text[3] = {amarokPlayerDCOP("artist"),
                       amarokPlayerDCOP("album"),
                       amarokPlayerDCOP("title")};
    
    // Check if the current text is the same
    QString textstr = text[0] + text[1] + text[2];
    if (textstr == m_textstr)
        return;
    m_textstr = textstr;
    
    // Build a path of the text to be displayed
    QPainterPath textPath;
    QFont font("Verdana", 26, QFont::Bold);
    qreal pos = 0;
    for (int i = 0; i < 3; i++) {
        textPath.addText(0, pos, font, text[i]);
        pos = textPath.boundingRect().height() + 10;
    }

    // Paint the text path onto a pixmap
    QPixmap buf(textPath.boundingRect().size().toSize());
    buf.fill(QColor(0,0,0,0));
    QPainter painter(&buf);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform |
                           QPainter::HighQualityAntialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.translate(-textPath.boundingRect().topLeft());
    painter.drawPath(textPath);
    painter.end();

    // Update the QGraphicsItem to display the new text pixmap
    m_text = buf;
    m_textitem.setPixmap(m_text);
    m_textitem.setPos(m_size + 10, (m_size - m_text.height())/2);
}


void NowPlayingItem::updateArtwork() {
    QString artworkstr = amarokPlayerDCOP("coverImage");

    // Check if the current image is the same
    if (artworkstr == m_artworkstr)
        return;
    m_artworkstr = artworkstr;

    // Get the new image
    QPixmap pixmap(artworkstr);
    m_artwork = pixmap.scaled(QSize(m_size, m_size),
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation)
        .copy(0, 0, m_size, m_size);
    
    // Update the QGraphicsItem to display the new artwork pixmap
    m_artworkitem.setPixmap(m_artwork);
}

QString NowPlayingItem::amarokPlayerDCOP(const QString &func) const {
    QStringList arguments;
    arguments << "amarok" << "player" << func;
        
    QProcess dcop;
    dcop.start("dcop", arguments);

    if (!dcop.waitForFinished())
        return "";

    QTextCodec *codec = QTextCodec::codecForLocale();
    QString encoded = codec->toUnicode(dcop.readAllStandardOutput());
    return encoded.trimmed();
}
