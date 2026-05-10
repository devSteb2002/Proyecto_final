#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "character.h"
#include <QTimer>

class Projectile : public QObject, public QGraphicsPixmapItem{
    public:
        Projectile(Character* owner, PhysicsSystem* physics,  QString type, float px, float py);
        Projectile(float px, float py);

        void initProjectile();

        ~Projectile();
        bool getIsMoving() const;
        void setIsMoving(bool newIsMoving);

    private:
        Character*                owner;
        PhysicsSystem*         physics;
        QVector<QPixmap>   Vfreames;
        unsigned short          indxFrame = 0;
        QString                     type;
        float                         px;
        float                         py;
        bool                         isMoving = false;
        QTimer*                    timer;


    private slots:
        void updateProjectile();
};

#endif // PROJECTILE_H
