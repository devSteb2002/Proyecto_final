#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include "projectile.h"
#include "physicssystem.h"
#include <QSoundEffect>
#include <QTimer>

class Enemy : public Character{

    Q_OBJECT

    public:
        Enemy();
        Enemy(bool world, QString typeEnemy, float px, float py, bool upSideDown, bool first);
        Enemy(QString typeEnemy, float px, float py, bool world, QGraphicsScene *& scene, unsigned short learning);
        void intiEnemy();

        ~Enemy();

    private:
        bool        world;
        bool        upsideDown;
        bool        first;
        bool        isDead = false;
        QString   typeEnemy;
        QTimer*  timer = nullptr;
        QTimer* timerShoot = nullptr;
        QTimer* timerExploted = nullptr;
        QGraphicsScene* scene = nullptr;
        QVector<QPixmap> vFramesXploted;
        PhysicsSystem* physics = nullptr;
        QSoundEffect*  explosionSound = nullptr;
        unsigned short  learning = 1;

        void scheduleNexShoot();

    private slots:
        void frameFire();
        void framMCU();
        void shootThunder();
        void explote();

    signals:
        void damageToPlayer(unsigned short restHearts);
        void enemyKilled();
};

#endif // ENEMY_H
