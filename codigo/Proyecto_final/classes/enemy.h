#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "projectile.h"
#include <QSoundEffect>
#include <QTimer>

class Enemy : public Character{

    Q_OBJECT

    public:
        Enemy();
        Enemy(bool world, QString typeEnemy, float px, float py, bool upSideDown, bool first);
        Enemy(PhysicsSystem* physics, QString typeEnemy, float px, float py, bool world, QGraphicsScene *& scene);
        void intiEnemy();

        ~Enemy();
    private:
        bool        world;
        bool        upsideDown;
        bool        first;
        bool        isDead = false;
        QString   typeEnemy;
        QTimer*  timer;
        QTimer* timerShoot;
        QGraphicsScene* scene;
        QVector<QPixmap> vFramesXploted;

        void scheduleNexShoot();

    private slots:
        void frameFire();
        void framMCU();
        void shootThunder();

    public slots:
        void getDamage();

    signals:
        void damageToPlayer(unsigned short restHearts);
};

#endif // ENEMY_H
