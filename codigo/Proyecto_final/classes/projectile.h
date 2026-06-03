#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "character.h"
#include <QTimer>

class Projectile : public QObject, public QGraphicsPixmapItem{

    Q_OBJECT

    public:
        Projectile();
        Projectile(Character* owner, QString type, float px, float py, float anglef, float v0); // nivel 1
        Projectile(Character* owner, float px, float py, float v0); // nivel 2 lanzamiento del jugador
        Projectile(Character* owner, float px, float py, float v0, bool boos); // lanzmiento de robots
        Projectile(Character* owner, double px, double py); // rayo del boos
        Projectile(float px, float py);

        void initProjectile();

        bool getIsMoving() const;
        void setIsMoving(bool newIsMoving);

        ~Projectile();
    private:
        Character* owner = nullptr;
        PhysicsSystem* physics = nullptr;
        QVector<QPixmap> Vfreames;
        unsigned short indxFrame = 0;
        QString type;
        float px;
        float py;
        float v0;
        float angle = 50;
        bool isMoving = false;
        QTimer* timer = nullptr;
        QTimer* timerRotateLaser = nullptr;
        unsigned short frame = 0;
        float time = 0.0f;
        bool stoped = false;


    private slots:
        void updateProjectile();
        void shootProjectilePlayer();
        void shootThunder();
        void shoortLaser();

    signals:
        void portalTouched();
        void playerColision(unsigned short restHearts);
        void bossColision();
};

#endif // PROJECTILE_H
