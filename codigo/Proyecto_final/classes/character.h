#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "physicssystem.h"

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    public:
        Character();
        Character(PhysicsSystem* physics);

        virtual void reinitBall();
        virtual  void loseLife();

        ~Character();
    protected:
        unsigned short        frame = 0;
        unsigned short        life = 100;
        QAudioOutput*        audioOutput;
        QMediaPlayer*         sound;
        QVector<QPixmap>  vFrames;
        float                        px;
        float                        py;
        float                        angle = 0;
        short                       vPerFrame = 0;
        PhysicsSystem*        physics = nullptr;

};

#endif // CHARACTER_H
