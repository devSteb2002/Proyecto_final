#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>
#include <QSoundEffect>
#include "physicssystem.h"

class Character : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    public:
        Character();
        Character(PhysicsSystem* physics);

        virtual void reinitBall();
        virtual  void loseLife();
        virtual  void onPortalTouched();

        ~Character();
    protected:
        unsigned short         frame = 0;
        unsigned short         life = 100;
        QSoundEffect*          effect;
        QVector<QPixmap>  vFrames;
        QVector<QPixmap>  vFramesRunningRight;
        QVector<QPixmap>  vFramesRunningleft;
        bool                        direction;
        float                        px;
        float                        py;
        float                        angle = 0;
        short                       vPerFrame = 0;
        PhysicsSystem*         physics = nullptr;

};

#endif // CHARACTER_H
