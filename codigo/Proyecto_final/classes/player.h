#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "projectile.h"
#include <Qtimer>

class Player :  public Character {

    Q_OBJECT

    public:
        Player();
        Player(QGraphicsScene*& scene, unsigned short level);

        void initPlayer();
        void initPlayer2();
        void reinitBall() override;
        void loseLife() override;
        void onPortalTouched() override;


        ~Player();
    private:
        unsigned short                            attempts = 3;
        unsigned short                             force;
        QGraphicsScene*                          scene;
        QVector<QGraphicsPixmapItem*>   hearts;
        QVector<QPixmap>                       portals;
        QGraphicsPixmapItem*                   portal;
        QTimer*                                      timerPortal;
        unsigned short                              framePortal = 0;
        QGraphicsRectItem*                      powerBg;
        QGraphicsRectItem*                      powerBar;
        bool                                            charging;
        QTimer*                                      timer;
        bool                                            isShooting = false;
        Projectile*                                    staticProjectile = nullptr;
        Projectile*                                    movingProjectile = nullptr;
        QGraphicsPixmapItem*                   arrow =nullptr;
        float                                            angleArrow = 0.0f;
        bool                                            isArrowUp = true;
        unsigned short                              frameArrow = 0;
        bool                                            isLoose = false;
        QSoundEffect*                              hitBall;
        unsigned short                              level = 1;
        bool                                            isMoving = false;

    protected:
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

    private slots:
        void  updatePlayer();
        void  updatePortal();

    public slots:
        void  getDamage();

    signals:
        void levelCompleted();
        void damageToBoss();

};


#endif // PLAYER_H
