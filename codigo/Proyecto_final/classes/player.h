#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include <Qtimer>

class Player :  public Character {

    public:
        Player(QGraphicsScene*& scene);

        void initPlayer();


        ~Player();
    private:
        unsigned short                               attempts = 3;
        unsigned short                               force;
        QGraphicsScene*                           scene;
        QVector<QGraphicsPixmapItem*>   hearts;
        QGraphicsRectItem*                       powerBg;
        QGraphicsRectItem*                       powerBar;
        bool                                              charging;
        QTimer*                                         timer;

    protected:
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

    private slots:
        void updatePlayer();

};


#endif // PLAYER_H
