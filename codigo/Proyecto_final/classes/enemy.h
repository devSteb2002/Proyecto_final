#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <QTimer>

class Enemy : public Character{
    public:
        Enemy(bool world, QString typeEnemy, float px, float py);
        void intiEnemy();

        ~Enemy();
    private:
        bool        world;
        QString   typeEnemy;
        QTimer*  timer;

    private slots:
        void frameFire();
};

#endif // ENEMY_H
