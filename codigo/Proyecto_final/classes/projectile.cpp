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

    this->timer = new QTimer();
    connect(this->timer, &QTimer::timeout, this, &Projectile::updateProjectile);
    this->timer->start(14);

    this->physics = new PhysicsSystem();
    this->physics->setX0(this->px);
    this->physics->setY0(this->py);

    this->v0 = (this->v0 * 150.0f) / 480.0f;
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
    if (this->isMoving){

        this->frame++;
        if  (this->frame >= 25){
            this->frame = 0;
            setPixmap(this->Vfreames[this->indxFrame]);
            this->indxFrame = (this->indxFrame+ 1) %this->Vfreames.size();
        }
    }

    QList<QGraphicsItem*> collisions = collidingItems();

    for (QGraphicsItem* item : std::as_const(collisions)){
        qDebug() << item->data(0).toString() << "\n";

        if (item->data(0).toString() == "platform"){

            this->physics->setX0(this->px - 5);
            this->physics->setY0(this->py);

            this->angle = 180.0f - this->angle;
            this->time = 0.0f;

            this->v0 = this->v0 - 50;

            setPos(this->px, this->py);

            break;
        }
        else if (item->data(0).toString() == "wall"){

        }
    }


    this->physics->parabolicMotion(this->px, this->py, this->angle, this->time, this->v0);
    this->time +=  0.1f;

    setPos(this->px, this->py);

    if (this->py > scene()->height() || this->px > scene()->width() || this->px + boundingRect().width() < 0){ // pelota salio, perder vida y reiniciar pelota
        this->owner->reinitBall();
        this->owner->loseLife();
        return;
    }
}


Projectile::~Projectile(){
    delete this->timer;
    delete this->physics;
}

bool Projectile::getIsMoving() const {
    return isMoving;
}

void Projectile::setIsMoving(bool newIsMoving){
    isMoving = newIsMoving;
}
