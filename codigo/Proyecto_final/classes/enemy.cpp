#include "enemy.h"
#include <qgraphicsscene.h>

Enemy::Enemy(bool world, QString typeEnemy, float px, float py) : world(world), typeEnemy(typeEnemy) {

    Character::audioOutput = new QAudioOutput(this);
    Character::sound = new QMediaPlayer(this);
    Character::sound->setAudioOutput(Character::audioOutput);
    Character::sound->setSource(QUrl("qrc:/audio/fire.mp3"));
    Character::audioOutput->setVolume(0.1);
    Character::sound->play();
    Character::sound->setLoops(QMediaPlayer::Infinite);

    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, &Enemy::frameFire);
    timer->start(200);


    Character::px = px;
    Character::py = py;
}

void Enemy::intiEnemy(){
    if (this->world){
        QPixmap sheet(":/images/sonic-scrap_.png");

        if (this->typeEnemy == "fire"){
            unsigned short px = 610;
            for (short i = 0; i < 4; i++){
                Character::vFrames.push_back(sheet.copy(px, 670, 30, 100).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                px += 30;
            }
        }
    }
}

void Enemy::frameFire(){
    setPixmap(Character::vFrames[frame]);
    setPos(Character::px, Character::py);
    frame = (frame + 1) % Character::vFrames.size();
}

Enemy::~Enemy(){
    delete this->timer;
}
