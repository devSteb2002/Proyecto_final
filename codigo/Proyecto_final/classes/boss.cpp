#include "boss.h"
#include <qgraphicsscene.h>

Boss::Boss() {}

Boss::Boss(float px, float py, Player *player, QGraphicsScene *&scene) : player(player), scene(scene) {

    Character::px = px;
    Character::py = py;

    this->laught = new QSoundEffect();
    this->laught->setSource(QUrl("qrc:/audio/laught.wav"));
    this->laught->setVolume(1.0f);

    this->rocket = new QSoundEffect();
    this->rocket->setSource(QUrl("qrc:/audio/rocket.wav"));
    this->rocket->setVolume(0.4f);
    this->rocket->setLoopCount(QSoundEffect::Infinite);
    this->rocket->play();

    this->scape = new QSoundEffect();
    this->scape->setSource(QUrl("qrc:/audio/scream.wav"));
    this->scape->setVolume(0.8f);

    this->moved = new QTimer();
    connect(this->moved, &QTimer::timeout, this, &Boss::updateFrame);
    this->moved->start(200);

    this->laughtTimer = new QTimer();
    connect(this->laughtTimer, &QTimer::timeout, this, &Boss::laughtAction);
    this->laughtTimer->start(6000);
}


void Boss::initBoss(){
    QPixmap sheetEnemy(":/images/sonic_enemy.png");

    Character::vFrames = {
        sheetEnemy.copy(0, 0, 380, 560).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheetEnemy.copy(385, 0, 380, 560).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    setPos(Character::px, Character::py);
}

void Boss::updateFrame(){
    setPixmap(Character::vFrames[Character::frame]);
    Character::frame = (Character::frame + 1) % Character::vFrames.size();


    if (this->isScape){
        Character::px += 4;
        setPos(Character::px, Character::py);

        if (Character::px > this->scene->width()){
            emit youWin();
            this->scene->removeItem(this);
            deleteLater();
        }
    }
}

void Boss::laughtAction(){
    if (this->isScape) {
        this->scape->play();
        return;
    }
    this->laught->play();

    //lanzar laser
    // Projectile* laser = new Projectile(this, Character::px+ 90, Character::py + 80);
    // Projectile* lase2 = new Projectile(this, Character::px, Character::py + 80);
    // this->scene->addItem(laser);
    // this->scene->addItem(lase2);

    // QTimer::singleShot(3000, [this, laser, lase2](){

    //     this->scene->removeItem(laser);
    //     this->scene->removeItem(lase2);

    //     delete laser;
    //     delete lase2;
    // });
}

void Boss::scapeBoss(){
    this->laught->stop();
    this->isScape = true;
    this->moved->start(14);
}

Boss::~Boss(){
    Character::vFrames.clear();
    if (this->moved != nullptr) delete this->moved;
    if (this->laught != nullptr) delete this->laught;
    if (this->rocket != nullptr) delete this->rocket;
    if (this->laughtTimer != nullptr) delete this->laughtTimer;
    if (this->scape != nullptr) delete this->scape;
}
