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
#include "boss.h"
#include "savemanager.h"

class Game: public QWidget{

    Q_OBJECT

public:
    Game();
    explicit Game(QWidget* parent = nullptr);
    ~Game();

private:

    QStackedWidget*     stack = nullptr;
    QWidget*                menuScreen = nullptr;
    QPixmap                  originalFrame;
    QLabel*                   bgMenu = nullptr;
    QSoundEffect*          theme = nullptr;
    QSoundEffect*          themeLevels = nullptr;
    QGraphicsScene*       scene = nullptr;
    QGraphicsView*         view = nullptr;
    Player*                     player = nullptr;
    SaveManager*           savemanager = nullptr;
    unsigned short           enemiesAlive = 4;
    Boss*                        boss = nullptr;

    void designLevel1();
    void designLevel2();
    void updateLevel();

private slots:
    void onEnemyKilled();
    void winGame();
    void loseLevel();
};

#endif // GAME_H
