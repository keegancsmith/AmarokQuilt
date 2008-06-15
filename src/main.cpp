#include <QApplication>
#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDir>
#include <QDebug>
#include <QTime>
#include <QProcess>
#include <QTextDocument>

#define SIZE 75
#define SIZEBIG 175
#define WIDTH SIZE * 10
#define HEIGHT SIZE * 7

class AlbumArtworkItem : public QGraphicsPixmapItem {
public:
    AlbumArtworkItem() {
        m_artwork = 0;
        updateArtwork();
    }    
    
private:
    void updateArtwork() {
        QDir dir("/home/keegan/.kde/share/apps/amarok/albumcovers/large/");
        QStringList coverList = dir.entryList(QDir::Files);
        int imageIndex = qrand() % coverList.size();

        QPixmap pixmap(dir.absoluteFilePath(coverList[imageIndex]));
        m_artwork = pixmap.scaled(QSize(SIZE,SIZE),
                                  Qt::KeepAspectRatioByExpanding,
                                  Qt::SmoothTransformation);
        
        
        setPixmap(m_artwork);
    }

    QPixmap m_artwork;
};

class NowPlayingItem : public QGraphicsItemGroup {
public:
    NowPlayingItem() {
        m_artworkitem.setParentItem(this);
        m_text.setParentItem(this);
        
        m_artworkitem.setPos(0,0);
        m_text.setPos(SIZEBIG + 10, 0);

        m_text.setFont(QFont("Helvetica", 14, QFont::Bold));

        updateNowPlaying();
    }


private:

    void updateNowPlaying() {
        if (amarokPlayerDCOP("isPlaying") != "true") {
            setVisible(false);
            return;            
        }
        setVisible(true);

        QString text = QString("<font color=\"white\">")
            + "<h1>" + Qt::escape(amarokPlayerDCOP("artist")) + "</h1>"
            + "<h2>" + Qt::escape(amarokPlayerDCOP("album"))  + "</h2>"
            + "<h2>" + Qt::escape(amarokPlayerDCOP("title"))  + "</h2>"
            + QString("</font>");
        m_text.setHtml(text);
        qDebug() << text;

        QPixmap pixmap(amarokPlayerDCOP("coverImage"));
        m_artwork = pixmap.scaled(QSize(SIZEBIG,SIZEBIG),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation);
        
        m_artworkitem.setPixmap(m_artwork);
    }

    QString amarokPlayerDCOP(const QString &func) const {
        QStringList arguments;
        arguments << "amarok" << "player" << func;
        
        QProcess dcop;
        dcop.start("dcop", arguments);

        if (!dcop.waitForFinished())
            return "";

        return QString(dcop.readAllStandardOutput()).trimmed();
    }

    QGraphicsTextItem m_text;
    QGraphicsPixmapItem m_artworkitem;
    QPixmap m_artwork;
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    
    QGraphicsScene scene;
    scene.setSceneRect(0,0,WIDTH,HEIGHT);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    
    for (int r = 0; r < WIDTH; r+=SIZE) {
        for (int c = 0; c < HEIGHT; c+= SIZE) {
            AlbumArtworkItem *cover = new AlbumArtworkItem();
            cover->setPos(r, c);
            scene.addItem(cover);
        }
    }

    NowPlayingItem nowPlaying;
    nowPlaying.setPos(100, 100);
    nowPlaying.setZValue(9999);
    scene.addItem(&nowPlaying);

    QGraphicsView view(&scene);
    view.setWindowState(view.windowState() | Qt::WindowFullScreen);
    // view.setRenderHint(QPainter::Antialiasing);
    // view.setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
    // view.setCacheMode(QGraphicsView::CacheBackground);
    // view.setDragMode(QGraphicsView::ScrollHandDrag);
    // view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
    //view.resize(WIDTH, HEIGHT);
    view.show();

    return app.exec();
}
