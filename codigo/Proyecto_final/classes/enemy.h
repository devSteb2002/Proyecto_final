#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <QTimer>

class Enemy : public Character{
    public:
        Enemy(bool world, QString typeEnemy, float px, float py, bool upSideDown, bool first);
        Enemy(PhysicsSystem* physics, QString typeEnemy, float px, float py, bool world);
        void intiEnemy();

        ~Enemy();
    private:
        bool        world;
        bool        upsideDown;
        bool        first;
        QString    typeEnemy;
        QTimer*  timer;

    private slots:
        void frameFire();
        void framMCU();
};

#endif // ENEMY_H
