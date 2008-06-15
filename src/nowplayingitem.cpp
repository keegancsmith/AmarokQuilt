#include "nowplayingitem.h"

#include <QPainterPath>
#include <QPainter>
#include <QFont>
#include <QProcess>

NowPlayingItem::NowPlayingItem() {
    m_artworkitem.setParentItem(this);
    m_textitem.setParentItem(this);
    m_artworkitem.setPos(0,0);

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
        
    QString textstr = text[0] + text[1] + text[2];
    if (textstr == m_textstr)
        return;
    m_textstr = textstr;
        
        
    QPainterPath textPath;
    QFont font("Verdana", 26, QFont::Bold);
    qreal pos = 0;
    for (int i = 0; i < 3; i++) {
        textPath.addText(0, pos, font, text[i]);
        pos = textPath.boundingRect().height() + 10;
    }

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

    m_text = buf;
    m_textitem.setPixmap(m_text);
    m_textitem.setPos(SIZEBIG + 10, (SIZEBIG - m_text.height())/2);
}


void NowPlayingItem::updateArtwork() {
    QString artworkstr = amarokPlayerDCOP("coverImage");
    if (artworkstr == m_artworkstr)
        return;
    m_artworkstr = artworkstr;

    QPixmap pixmap(artworkstr);
    m_artwork = pixmap.scaled(QSize(SIZEBIG,SIZEBIG),
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation)
        .copy(0, 0, SIZEBIG, SIZEBIG);
        
    m_artworkitem.setPixmap(m_artwork);
}

QString NowPlayingItem::amarokPlayerDCOP(const QString &func) const {
    QStringList arguments;
    arguments << "amarok" << "player" << func;
        
    QProcess dcop;
    dcop.start("dcop", arguments);

    if (!dcop.waitForFinished())
        return "";

    return QString(dcop.readAllStandardOutput()).trimmed();
}
