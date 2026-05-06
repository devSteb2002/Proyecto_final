#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QAudioOutput>

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    public:
        Character();

    protected:
        unsigned short        attempts;
        unsigned short        force;
        unsigned short        frame = 0;
        QAudioOutput*        audioOutput;
        QMediaPlayer*         sound;
        QVector<QPixmap>  vFrames;
        qreal                       px;
        qreal                       py;


};

#endif // CHARACTER_H
