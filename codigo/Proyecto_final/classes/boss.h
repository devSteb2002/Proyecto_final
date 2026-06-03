#ifndef BOSS_H
#define BOSS_H

#include "character.h"
#include "projectile.h"
#include "player.h"
#include <QSoundEffect>
#include <QTimer>

class Boss : public Character{

    Q_OBJECT

    public:
        Boss();
        Boss(float px, float py, Player* player, QGraphicsScene*& scene);

        void initBoss();
        void scapeBoss();

        ~Boss();
    private:
        QGraphicsScene* scene = nullptr;
        QSoundEffect* laught = nullptr;
        QSoundEffect* rocket = nullptr;
        QSoundEffect* scape = nullptr;
        QTimer*         timerShoot = nullptr;
        QTimer*         moved = nullptr;
        QTimer*         laughtTimer = nullptr;
        Player*           player = nullptr;
        bool               isScape = false;

    private slots:
        void updateFrame();
        void laughtAction();

    signals:
        void  youWin();

};

#endif // BOSS_H
