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
        void initPlayer(bool level2);
        void reinitBall() override;
        void loseLife() override;
        void onPortalTouched() override;


        ~Player();
        unsigned short getAttempts() const;
        void setAttempts(unsigned short newAttempts);

    private:
        unsigned short                             attempts = 3;
        unsigned short                             force = 0;
        QGraphicsScene*                          scene = nullptr;
        QVector<QGraphicsPixmapItem*>   hearts;
        QVector<QPixmap>                       portals;
        QGraphicsPixmapItem*                   portal = nullptr;
        QTimer*                                      timerPortal = nullptr;
        unsigned short                              framePortal = 0;
        QGraphicsRectItem*                      powerBg = nullptr;
        QGraphicsRectItem*                      powerBar = nullptr;
        bool                                            charging;
        QTimer*                                      timer = nullptr;
        bool                                            isShooting = false;
        Projectile*                                    staticProjectile = nullptr;
        Projectile*                                    movingProjectile = nullptr;
        QGraphicsPixmapItem*                   arrow =nullptr;
        float                                            angleArrow = 0.0f;
        bool                                            isArrowUp = true;
        unsigned short                              frameArrow = 0;
        bool                                            isLoose = false;
        QSoundEffect*                              hitBall = nullptr;
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
        void lose();

};


#endif // PLAYER_H
