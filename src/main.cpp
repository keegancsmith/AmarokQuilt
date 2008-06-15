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
#include <QDesktopWidget>

#include "nowplayingitem.h"

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
        
        qDebug() << "Loading " << dir.absoluteFilePath(coverList[imageIndex]);
        QPixmap pixmap(dir.absoluteFilePath(coverList[imageIndex]));
        m_artwork = pixmap.scaled(QSize(SIZE,SIZE),
                                  Qt::KeepAspectRatioByExpanding,
                                  Qt::SmoothTransformation);
        
        setPixmap(m_artwork);
    }

    QPixmap m_artwork;
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    QRect screenSize = QApplication::desktop()->screenGeometry();
    int width = screenSize.width();
    int height = screenSize.height();
    qreal wdelta = (width % SIZE) / 2;
    qreal hdelta = (height % SIZE) / 2;

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, width, height);
    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    
    for (int r = 0; r < width; r+=SIZE) {
        for (int c = 0; c < height; c+= SIZE) {
            AlbumArtworkItem *cover = new AlbumArtworkItem();
            cover->setPos(r - wdelta, c - hdelta);
            scene.addItem(cover);
        }
    }

    NowPlayingItem nowPlaying;
    nowPlaying.setPos(100, 100);
    nowPlaying.setZValue(9999);
    scene.addItem(&nowPlaying);

    QGraphicsView view(&scene);
    view.setWindowState(view.windowState() | Qt::WindowFullScreen);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // view.setRenderHint(QPainter::Antialiasing);
    // view.setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
    // view.setCacheMode(QGraphicsView::CacheBackground);
    // view.setDragMode(QGraphicsView::ScrollHandDrag);
    // view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
    //view.resize(WIDTH, HEIGHT);
    view.show();

    return app.exec();
}
