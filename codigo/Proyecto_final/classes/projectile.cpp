#include "projectile.h"
#include <qgraphicsscene.h>

Projectile::Projectile(Character *owner, QString type, float px, float py, float anglef, float v0)
    : owner(owner), type(type), px(px), py(py), angle(anglef), v0(v0) {

    if (this->type == "golf"){
        QPixmap sheet(":/images/ball_golf.png");
        QPixmap ball = sheet.copy(10, 10, 153, 200).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        this->Vfreames = {
            sheet.copy(10, 10, 153, 200).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation),
            sheet.copy(180, 10, 153, 200).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        };
    }

    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, &Projectile::updateProjectile);
    this->timer->start(14);

    this->physics = new PhysicsSystem();
    this->physics->setX0(this->px);
    this->physics->setY0(this->py);

    this->v0 = (this->v0 * 150.0f) / 480.0f;

    connect(this, &Projectile::portalTouched, this->owner, &Character::onPortalTouched);
}

Projectile::Projectile(Character* owner, float px, float py, float v0) : owner(owner), px(px), py(py), v0(v0){
    QPixmap sheet(":/images/ball_golf.png");
    QPixmap ball = sheet.copy(10, 10, 153, 200).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    this->Vfreames = {
        sheet.copy(10, 10, 153, 200).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(180, 10, 153, 200).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    this->timer = new QTimer();
    connect(this->timer, &QTimer::timeout, this, &Projectile::shootProjectilePlayer);
    this->timer->start(14);

    this->physics = new PhysicsSystem();
    this->physics->setX0(this->px);
    this->physics->setY0(this->py);

}

Projectile::Projectile(Character* owner, float px, float py, float v0, bool boos) : px(px), py(py){

    if (boos){
        QPixmap sheet_(":/images/robot.png");
        QPixmap thunder = sheet_.copy(135, 510, 100, 30).scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(90));;

        this->timer = new QTimer(this);
        connect(this->timer, &QTimer::timeout, this, &Projectile::shootThunder);
        this->timer->start(14);

        setPixmap(thunder);
        setPos(this->px, this->py);

    }
}


Projectile::Projectile(float px, float py){ // bola estatica solamente visual
    QPixmap sheet(":/images/ball_golf.png");
    QPixmap ball = sheet.copy(10, 10, 153, 200).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(ball);
    setPos(px, py);
}

void Projectile::initProjectile(){

    setPixmap(this->Vfreames[1]);
    setPos(this->px, this->py);
}

void Projectile::updateProjectile(){

    if (this->stoped) return;

    if (this->isMoving){

        this->frame++;
        if  (this->frame >= 25){
            this->frame = 0;
            setPixmap(this->Vfreames[this->indxFrame]);
            this->indxFrame = (this->indxFrame+ 1) %this->Vfreames.size();
        }
    }

    QList<QGraphicsItem*> collisions = collidingItems();
    bool isToSlow = false;
    bool ended     = false;

    for (QGraphicsItem* item : std::as_const(collisions)){
        qDebug() << item->data(0).toString() << "\n";

        if (item->data(0).toString() == "platform" || (item->data(0).toString() == "wall" && this->v0 > 100)){

            this->physics->setX0(this->px - 5);
            this->physics->setY0(this->py);

            this->angle = 180.0f - this->angle;
            this->time = 0.0f;

            this->v0 = this->v0 - 50;

            setPos(this->px, this->py);

            break;
        }
        else if (item->data(0).toString() == "wall"){
            this->owner->reinitBall();
            this->owner->loseLife();
            isToSlow = true;
            break;
        }
        else if (item->data(0).toString() == "portal"){
            this->stoped = true;
            ended = true;
            emit portalTouched();
            break;
        }
    }

    if (ended) return;
    if (isToSlow) return;


    this->physics->parabolicMotion(this->px, this->py, this->angle, this->time, this->v0);
    this->time +=  0.1f;

    setPos(this->px, this->py);

    if (this->py > scene()->height() || this->px > scene()->width() || this->px + boundingRect().width() < 0){ // pelota salio, perder vida y reiniciar pelota
        this->owner->reinitBall();
        this->owner->loseLife();
        return;
    }
}

void Projectile::shootProjectilePlayer(){

    this->physics->parabolicMotion(this->px, this->py, 90.0f, this->time, this->v0); // solo componente vertical
    this->time +=  0.1f;

    setPos(this->px, this->py);

    QList<QGraphicsItem*> collisions = collidingItems();

    bool colision = false;
    for (QGraphicsItem* item : std::as_const(collisions)){
        if (item->data(0).toString() == "boss"){
            emit bossColision();
            break;
        }
    }


    if (this->py >= scene()->height() - 10){
        scene()->removeItem(this);
        qDebug() << "projectile eliminado";
        delete this;
    }
}

void Projectile::shootThunder(){
    this->py = this->py + 10;
    setPos(this->px, this->py);

     QList<QGraphicsItem*> collisions = collidingItems();

    bool colision = false;
    for (QGraphicsItem* item : std::as_const(collisions)){
         if (item->data(0).toString() == "player"){

             this->timer->stop();
             this->timer->disconnect();

             scene()->removeItem(this);

             this->deleteLater();

             emit playerColision(1);
            colision = true;
            break;
         }
    }

    if (colision) return;

    if (!scene()) return;

    if (this->py >= scene()->height() - 10){
        this->timer->stop();
        this->timer->disconnect();

        scene()->removeItem(this);

        this->deleteLater();
        return;
    }
}

Projectile::~Projectile(){
    delete this->timer;

    if (this->physics != nullptr){
        delete this->physics;
        this->physics = nullptr;
    }
}

bool Projectile::getIsMoving() const {
    return isMoving;
}

void Projectile::setIsMoving(bool newIsMoving){
    isMoving = newIsMoving;
}
