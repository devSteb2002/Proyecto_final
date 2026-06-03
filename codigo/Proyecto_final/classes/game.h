#ifndef GAME_H
#define GAME_H

#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QSoundEffect>
#include "player.h"
#include "enemy.h"
#include "physicssystem.h"
#include "savemanager.h"

class Game: public QWidget{

    Q_OBJECT

public:
    Game();
    explicit Game(QWidget* parent = nullptr);
    ~Game();

private:

    QStackedWidget*     stack;
    QWidget*                menuScreen;
    QPixmap                  originalFrame;
    QLabel*                   bgMenu;
    QSoundEffect*          theme;
    QGraphicsScene*     scene;
    QGraphicsView*       view;
    QVector<Enemy*>   enemies;
    PhysicsSystem*        physics;
    Player*                     player;
    SaveManager*         savemanager;

    void designLevel1(QGraphicsScene*& scene);
    void designLevel2(QGraphicsScene*& scene);
    void updateLevel();
};

#endif // GAME_H
