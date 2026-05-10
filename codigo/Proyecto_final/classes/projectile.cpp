#include "projectile.h"

Projectile::Projectile(Character *owner, PhysicsSystem *physics, QString type, float px, float py) : owner(owner), type(type), px(px), py(py) {

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

        setPixmap(this->Vfreames[this->indxFrame]);
        this->indxFrame = (this->indxFrame+ 1) %this->Vfreames.size();
    }
}


Projectile::~Projectile(){
    delete this->timer;
}

bool Projectile::getIsMoving() const {
    return isMoving;
}

void Projectile::setIsMoving(bool newIsMoving){
    isMoving = newIsMoving;
}
