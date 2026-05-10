#include "enemy.h"
#include <qgraphicsscene.h>
#include <QRandomGenerator>

Enemy::Enemy(bool world, QString typeEnemy, float px, float py, bool upSideDown, bool first)
    : world(world), typeEnemy(typeEnemy), upsideDown(upSideDown), first(first) {

    if (this->first && this->typeEnemy == "fire"){
        Character::audioOutput = new QAudioOutput(this);
        Character::sound = new QMediaPlayer(this);
        Character::sound->setAudioOutput(Character::audioOutput);
        Character::sound->setSource(QUrl("qrc:/audio/fire.mp3"));
        Character::audioOutput->setVolume(0.1);
        Character::sound->play();
        Character::sound->setLoops(QMediaPlayer::Infinite);
    }

    this->timer = new QTimer();

    if (this->typeEnemy == "fire") connect(this->timer, &QTimer::timeout, this, &Enemy::frameFire);

    timer->start(300);

    Character::px = px;
    Character::py = py;
}

Enemy::Enemy(PhysicsSystem* physics, QString typeEnemy, float px, float py, bool world)
    : world(world), typeEnemy(typeEnemy), Character(physics) {

    if (this->typeEnemy == "robot") {
        this->timer = new QTimer();
        connect(this->timer, &QTimer::timeout, this, &Enemy::framMCU);
        timer->start(16);
    }


    Character::px = px;
    Character::py = py;
}


void Enemy::intiEnemy(){
    if (this->world){
        if (this->typeEnemy == "fire"){
            QPixmap sheet(":/images/sonic-scrap_.png");
            unsigned short px = 610;

            for (short i = 0; i < 4; i++){

                QPixmap frame = sheet.copy(px, 670, 30, 100).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                if (this->upsideDown) frame = frame.transformed(QTransform().rotate(180));

                Character::vFrames.push_back(frame);
                px += 30;
            }
        }
        else if (this->typeEnemy == "robot"){
            QPixmap sheet(":/images/robot.png");
            QPixmap frame = sheet.copy(6, 14, 20, 30).scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            setPixmap(frame);
            setPos(500, 500);
        }
    }
}

void Enemy::frameFire(){
    Character::frame = QRandomGenerator::global()->bounded(vFrames.size());
    setPixmap(Character::vFrames[frame]);
    setPos(Character::px, Character::py);
    frame = (frame + 1) % Character::vFrames.size();
}

void Enemy::framMCU(){

    Character::vPerFrame++;

    if(Character::vPerFrame >= 8){
        Character::vPerFrame = 0;
        Character::angle += 0.5;
    }

    Character::physics->mcu(Character::px , Character::py, 130, Character::angle);
    setPos(Character::px, Character::py);
}

Enemy::~Enemy(){
    delete this->timer;
}
