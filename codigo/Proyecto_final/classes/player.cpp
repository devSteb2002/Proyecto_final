#include "player.h"
#include <QPixmap>
#include <qgraphicsscene.h>
#include <QKeyEvent>

Player::Player(QGraphicsScene *&scene) : scene(scene) { //sonic

    //generar corazon y barra de potencia
    QPixmap heart(":/images/heart.png");
    QPixmap heartFormat = heart.scaled(50, 50,  Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    unsigned short width = 10;
    for (short i = 0; i < 3; i++){
        QGraphicsPixmapItem* hearItem = new QGraphicsPixmapItem(heartFormat);
        hearItem->setPos(width, 10);

       this->hearts.push_back(hearItem);
       this->scene->addItem(hearItem);

        width += 50;
    }

    this->powerBg = new QGraphicsRectItem(0, 0, 15, 400);
    this->powerBar = new QGraphicsRectItem(0, 0, 15, 100);
    this->powerBg->setBrush(Qt::darkGray);
    this->powerBar->setBrush(Qt::red);
    this->powerBg->setPos(30, 80);
    this->powerBar->setPos(50, 480);
    this->scene->addItem(this->powerBg);
    this->scene->addItem(this->powerBar);

    this->timer = new QTimer();
    connect(this->timer, &QTimer::timeout, this, &Player::updatePlayer);
    this->timer->start(14);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();


}

void Player::initPlayer(){

}

void Player::updatePlayer(){
    if (this->charging){
        this->force += 2;

        if (this->force > 480) this->force = 480;

        this->powerBar->setRect(0,0, 15, this->force);
    }

}

void Player::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space){
        this->charging = true;
    }
}

void Player::keyReleaseEvent(QKeyEvent* event) {

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Space){
        //--------------------------------
        //aca disparamos
        //---------------------
        this->charging = false;
        this->force = 0;
    }

}

Player::~Player(){
    delete this->timer;
}
