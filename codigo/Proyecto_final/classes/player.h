#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "projectile.h"
#include <Qtimer>

class Player :  public Character {

    public:
        Player(QGraphicsScene*& scene);

        void initPlayer();
        void reinitBall() override;
        void loseLife() override;


        ~Player();
    private:
        unsigned short                               attempts = 3;
        unsigned short                               force;
        QGraphicsScene*                            scene;
        QVector<QGraphicsPixmapItem*>     hearts;
        QVector<QPixmap>                        portals;
        QGraphicsPixmapItem*                    portal;
        QTimer*                                       timerPortal;
        unsigned short                               framePortal = 0;
        QGraphicsRectItem*                        powerBg;
        QGraphicsRectItem*                        powerBar;
        bool                                              charging;
        QTimer*                                        timer;
        bool                                              isShooting = false;
        Projectile*                                      staticProjectile = nullptr;
        Projectile*                                      movingProjectile = nullptr;
        QGraphicsPixmapItem*                     arrow =nullptr;
        float                                              angleArrow = 0.0f;
        bool                                              isArrowUp = true;
        unsigned short                                frameArrow = 0;

    protected:
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

    private slots:
        void updatePlayer();
        void  updatePortal();

};


#endif // PLAYER_H
