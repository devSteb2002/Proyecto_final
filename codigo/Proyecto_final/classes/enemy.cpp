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

Enemy::Enemy(QString typeEnemy, float px, float py, bool world, QGraphicsScene *&scene, unsigned short learning)
    : world(world), typeEnemy(typeEnemy), scene(scene), learning(learning) {

    this->physics = new PhysicsSystem();

    if (this->typeEnemy == "robot") {
        this->timer = new QTimer();
        connect(this->timer, &QTimer::timeout, this, &Enemy::framMCU);
        timer->start(16);

        this->timerShoot = new QTimer();
        connect(this->timerShoot, &QTimer::timeout, this, &Enemy::shootThunder);

        scheduleNexShoot();

        Character::effect = new QSoundEffect(this);
        Character::effect->setSource(QUrl("qrc:/audio/shoot.wav"));
        Character::effect->setVolume(0.3f);

        this->explosionSound = new QSoundEffect(this);
        this->explosionSound->setSource(QUrl("qrc:/audio/explosion.wav"));
        this->explosionSound->setVolume(1.0f);

        //-------- frames de muertes ------------- //
        QPixmap sheet(":/images/robot.png");

        this->vFramesXploted = {
            sheet.copy(0, 255, 30, 30).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            sheet.copy(33, 255, 30, 30).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            sheet.copy(70, 250, 35, 30).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            sheet.copy(110, 250, 35, 30).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };

        this->timerExploted = new QTimer();
        connect(this->timerExploted, &QTimer::timeout, this, &Enemy::explote);
        this->timerExploted->start(40);
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

    this->physics->mcu(Character::px , Character::py, 3, Character::angle);
    setPos(Character::px, Character::py);
}

void Enemy::shootThunder(){

    Projectile* projectileBoos = new Projectile(nullptr, Character::px, Character::py, 200, true);
    this->scene->addItem(projectileBoos);
    Character::effect->play();

    connect(projectileBoos, &Projectile::playerColision, this, &Enemy::damageToPlayer);
}

void Enemy::scheduleNexShoot(){
    if (this->learning == 0){
        unsigned short delay = QRandomGenerator::global()->bounded(1000,5000);
        this->timerShoot->start(delay);
    }
    else if (this->learning == 1){
        unsigned short delay = QRandomGenerator::global()->bounded(1000,2200);
        this->timerShoot->start(delay);
    }
    else {
        unsigned short delay = QRandomGenerator::global()->bounded(1000,1500);
        this->timerShoot->start(delay);
    }
}

void Enemy::explote(){

    if (this->isDead){
        Character::frame++;

        if (Character::frame >= this->vFramesXploted.size()){
            this->timerExploted->stop();

            emit enemyKilled();

            this->scene->removeItem(this);
            deleteLater();
            return;
        }

        setPixmap(this->vFramesXploted[Character::frame]);
    }

    if (this->isDead) return;

    QList<QGraphicsItem*> collisions = collidingItems();

    for (QGraphicsItem* item : std::as_const(collisions)){

        Projectile* projectile = dynamic_cast<Projectile*>(item);

        if (projectile && (projectile->data(0).toString() != "thunder" && projectile->data(0).toString() != "laser")){
            this->isDead = true;
            this->explosionSound->play();
            this->scene->removeItem(projectile);
            projectile->deleteLater();
            break;
        }
    }
}


Enemy::~Enemy(){
    if (this->timer != nullptr)   delete this->timer;
    if (this->timerShoot != nullptr) delete this->timerShoot;
    if (this->timerExploted != nullptr) delete this->timerExploted;
    if (this->physics != nullptr) delete this->physics;
    if (Character::effect != nullptr) delete Character::effect;
}
