#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "character.h"
#include <QTimer>

class Projectile : public QObject, public QGraphicsPixmapItem{
    public:
        Projectile(Character* owner, QString type, float px, float py, float anglef, float v0);
        Projectile(float px, float py);

        void initProjectile();

        bool getIsMoving() const;
        void setIsMoving(bool newIsMoving);

        ~Projectile();
    private:
        Character*                owner;
        PhysicsSystem*         physics;
        QVector<QPixmap>   Vfreames;
        unsigned short          indxFrame = 0;
        QString                     type;
        float                         px;
        float                         py;
        float                         v0;
        float                         angle = 50;
        bool                         isMoving = false;
        QTimer*                   timer;
        unsigned short          frame = 0;
        float                         time = 0.0f;


    private slots:
        void updateProjectile();
};

#endif // PROJECTILE_H
