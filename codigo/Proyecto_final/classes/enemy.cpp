#include "enemy.h"
#include <qgraphicsscene.h>
#include <QRandomGenerator>

Enemy::Enemy(){}

Enemy::Enemy(bool world, QString typeEnemy, float px, float py, bool upSideDown, bool first)
    : world(world), typeEnemy(typeEnemy), upsideDown(upSideDown), first(first) {

    if (this->first && this->typeEnemy == "fire"){
        Character::effect = new QSoundEffect(this);
        Character::effect->setSource(QUrl("qrc:/audio/fire.wav"));
        Character::effect->setVolume(0.1f);
        Character::effect->setLoopCount(QSoundEffect::Infinite);
        Character::effect->play();
    }

    this->timer = new QTimer();

    if (this->typeEnemy == "fire") connect(this->timer, &QTimer::timeout, this, &Enemy::frameFire);

    timer->start(300);

    Character::px = px;
    Character::py = py;
}

Enemy::Enemy(PhysicsSystem* physics, QString typeEnemy, float px, float py, bool world, QGraphicsScene *&scene)
    : world(world), typeEnemy(typeEnemy), Character(physics), scene(scene) {

    if (this->typeEnemy == "robot") {
        this->timer = new QTimer();
        connect(this->timer, &QTimer::timeout, this, &Enemy::framMCU);
        timer->start(16);

        this->timerShoot = new QTimer();
        connect(this->timerShoot, &QTimer::timeout, this, &Enemy::shootThunder);

        //shootThunder();
        scheduleNexShoot();

        Character::effect = new QSoundEffect(this);
        Character::effect->setSource(QUrl("qrc:/audio/shoot.wav"));
        Character::effect->setVolume(1.0f);

        //-------- frames de muertes ------------- //



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
            setPos(Character::px, Character::py);
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

    if(Character::vPerFrame >= 6){
        Character::vPerFrame = 0;
        Character::angle += 0.5;
    }

    Character::physics->mcu(Character::px , Character::py, 3, Character::angle);
    setPos(Character::px, Character::py);
}

void Enemy::shootThunder(){

    Projectile* projectileBoos = new Projectile(nullptr, Character::px, Character::py, 200, true);
    this->scene->addItem(projectileBoos);
    Character::effect->play();

    connect(projectileBoos, &Projectile::playerColision, this, &Enemy::damageToPlayer);
}

void Enemy::scheduleNexShoot(){
    unsigned short delay = QRandomGenerator::global()->bounded(1000,5000);
    this->timerShoot->start(delay);
}

void Enemy::getDamage(){




}

Enemy::~Enemy(){
    if (this->timer != nullptr)   delete this->timer;
    if (this->timerShoot != nullptr) delete this->timerShoot;
    if (Character::effect != nullptr) delete Character::effect;
}
