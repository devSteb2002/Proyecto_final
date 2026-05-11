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
        QString cont = "heart" + QString::number(i + 1);
        hearItem->setData(0, cont);
        hearItem->setPos(width, 10);

       this->hearts.push_back(hearItem);
       this->scene->addItem(hearItem);

        width += 50;
    }

    this->powerBg = new QGraphicsRectItem(0, 0, 15, 400);
    this->powerBar = new QGraphicsRectItem(0, 0, 15, 0);
    this->powerBg->setBrush(Qt::darkGray);
    this->powerBar->setBrush(Qt::red);
    this->powerBg->setPos(30, 80);
    this->powerBar->setPos(30, 480);
    this->scene->addItem(this->powerBg);
    this->scene->addItem(this->powerBar);

    this->timer = new QTimer();
    connect(this->timer, &QTimer::timeout, this, &Player::updatePlayer);
    this->timer->start(14);

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    this->staticProjectile = new Projectile(245, 709);
    this->staticProjectile->setData(0, "staticBall");
    this->scene->addItem(this->staticProjectile);

    Character::audioOutput = new QAudioOutput(this);
    Character::sound = new QMediaPlayer(this);
    Character::sound->setAudioOutput(Character::audioOutput);
    Character::sound->setSource(QUrl("qrc:/audio/golf_swing.mp3"));
    Character::audioOutput->setVolume(1.0);

}

void Player::initPlayer(){
    QPixmap sheet(":/images/sonic.png");
    QPixmap sonic = sheet.copy(0, 20, 32, 50).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    setPixmap(sonic);

    Character::vFrames = {
        sheet.copy(0, 19, 32, 50).scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(34, 19, 23, 50).scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(58, 19, 32, 50).scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(90, 19, 32, 50).scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(122, 19, 31, 50).scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation),
       // sheet.copy(153, 20, 31, 50).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation),
    };

    setPos(150, 635);
}

void Player::updatePlayer(){
    if (this->charging){ // barra de potencia

        if (this->powerBar->rect().height() <= 400) {
            this->force += 2;
            if (this->force > 480) this->force = 480;
            this->powerBar->setRect(0,  -this->force, 15, this->force);
        }
    }
    else this->powerBar->setRect(50, 480, 15, 0);

    if (Character::vFrames.empty()) return;

    if (this->charging){
        Character::vPerFrame++;

        if(Character::vPerFrame >= 18){
            bool isShooting = false;

          //  for (QGraphicsItem* item: this->scene->items()) if (item->data(0).toString() == "ballShooting") isShooting = true;
            if (this->movingProjectile != nullptr && this->movingProjectile->scene()) isShooting = true;

            if (this->powerBar->rect().height() <= 400 && !isShooting){
                Character::vPerFrame = 0;
                setPixmap(Character::vFrames[Character::frame]);
                Character::frame = (Character::frame + 1) % Character::vFrames.size();
            }
            else{
                setPixmap(Character::vFrames[Character::vFrames.size() - 1]);
            }
        }
    }else{
        QPixmap sheet(":/images/sonic.png");
        QPixmap sonic = sheet.copy(153, 19, 31, 50).scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(sonic);
    }

}

void Player::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space) this->charging = true;
}

void Player::keyReleaseEvent(QKeyEvent* event) {

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Space){
        if (!this->isShooting){
            this->staticProjectile->hide();
            bool findBallShooted = false;

            if (this->movingProjectile)  findBallShooted = true;

            if (!findBallShooted){
                this->movingProjectile = new Projectile(this, "golf", 245, 709);
                this->scene->addItem(this->movingProjectile);
                this->movingProjectile->setData(0, "ballShooting");
                this->movingProjectile->initProjectile();
                this->movingProjectile->setIsMoving(true);

                Character::sound->play();
            }
        }

        this->charging = false;
        this->force = 0;
    }
}


void Player::reinitBall(){ // vuelve a mostrar la bola estatica para que lance de nuevo
    this->staticProjectile->show();
    this->scene->removeItem(this->movingProjectile);
    delete this->movingProjectile;
    this->movingProjectile = nullptr;
    this->isShooting = false;
}

void Player::loseLife(){
    this->attempts -= 1;
    this->hearts[this->attempts]->hide();


    if (this->attempts == 0){ // perdio y se reinicia la vida para que vuelva a empezar
        qDebug() << "Perdiste"; //
        return;
    }
}


Player::~Player(){
    delete this->timer;
}
