#include "player.h"
#include <QPixmap>
#include <qgraphicsscene.h>
#include <QKeyEvent>

Player::Player(QGraphicsScene *&scene, unsigned short level) : scene(scene), level(level) { //sonic

    //------------- Vida y corazones-------------------------//
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


    this->timer = new QTimer();
    connect(this->timer, &QTimer::timeout, this, &Player::updatePlayer);

    if (this->level == 1){

       //----------- portal---------------------------------//
        QPixmap portals(":/images/portal.png");
        QPixmap portalUp1 = portals.copy(0, 0, 24, 31).scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(90));;
        QPixmap portalUp2 = portals.copy(30, 0, 25, 31).scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(90));;
        QPixmap portalUp3 = portals.copy(60, 0, 28, 31).scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(90));;

        QPixmap portalDo1 = portals.copy(0, 31, 24, 33).scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(90));;
        QPixmap portalDo2 = portals.copy(30, 31, 25, 33).scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(90));;
        QPixmap portalDo3 = portals.copy(60, 31, 28, 33).scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(90));;

        this->portals = {
            portalUp1,
            portalUp2,
            portalUp3,
            portalDo1,
            portalDo2,
            portalDo3
        };

        this->portal = this->scene->addPixmap(this->portals[0]);
        this->portal->setData(0, "portal");
        this->timerPortal = new QTimer();
        connect(this->timerPortal, &QTimer::timeout, this, &Player::updatePortal);
        timerPortal->start(200);

        //------------ Barra de fuerza--------------------//

        this->powerBg = new QGraphicsRectItem(0, 0, 15, 400);
        this->powerBar = new QGraphicsRectItem(0, 0, 15, 0);
        this->powerBg->setBrush(Qt::darkGray);
        this->powerBar->setBrush(Qt::red);
        this->powerBg->setPos(30, 80);
        this->powerBar->setPos(30, 480);
        this->scene->addItem(this->powerBg);
        this->scene->addItem(this->powerBar);

        //------------proyectil---------------------//

        this->staticProjectile = new Projectile(245, 709);
        this->staticProjectile->setData(0, "staticBall");
        this->scene->addItem(this->staticProjectile);


        //---------------- flecha---------------------//

        QPixmap arrow_(":/images/arrow.png");
        arrow_ = arrow_.copy(0, 20, 102, 100).scaled(50, 500, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->arrow = new QGraphicsPixmapItem(arrow_);
        this->arrow->setPos(380, 600);
        //this->arrow->setTransformOriginPoint(boundingRect().center());
        this->arrow->setRotation(180);
        this->arrow->hide();
        this->scene->addItem(this->arrow);


        this->timer->start(14);
    }
    else if (this->level == 2){

        this->timer->start(150);
        Character::px =500;
        Character::py = 756;
    }

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();


    //--------------audio-------------------------//

    this->hitBall = new QSoundEffect(this);
    this->hitBall->setSource(QUrl("qrc:/audio/golf_swing.wav"));
    this->hitBall->setVolume(1.0f);
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
    };

    setPos(150, 635);
}

void Player::initPlayer2(){
    QPixmap sheet(":/images/sonic_movement.png");

    Character::vFrames = {
        sheet.copy(1, 10, 30, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(30, 10, 25, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(55, 10, 30, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(85, 10, 25, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    Character::vFramesRunningRight = {
        sheet.copy(290, 60, 46, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(336, 60, 38, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(376, 60, 40, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation),
        sheet.copy(416, 60, 38, 43).scaled(95, 95, Qt::KeepAspectRatio, Qt::SmoothTransformation)
    };

    for (const QPixmap& pixmap : Character::vFramesRunningRight){
        Character::vFramesRunningleft.push_back(
            pixmap.transformed(QTransform().scale(-1, 1))
        );
    }

}


void Player::updatePlayer(){

    if (this->level == 1){ // nivel 1
        if (this->isLoose) return;

        if (this->charging){ // barra de potencia

            if (this->powerBar->rect().height() <= 400) {
                this->force += 2;
                if (this->force > 480) this->force = 480;
                this->powerBar->setRect(0,  -this->force, 15, this->force);
            }
            // else {
            //     this->powerBar->setRect(50, 480, 15, 0);
            //     this->force = 0;
            // }
        }
        else this->powerBar->setRect(50, 480, 15, 0);

        if (Character::vFrames.empty()) return;

        if (this->charging){
            Character::vPerFrame++;

            if(Character::vPerFrame >= 18){
                bool isShooting = false;

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


        this->frameArrow++;
        if (this->frameArrow >= 8){
            if (this->angleArrow >= 70.0f) this->isArrowUp = false;
            else if (this->angleArrow <= 0.0f) this->isArrowUp = true;

            if (this->isArrowUp) this->angleArrow += 10.0f;
            else this->angleArrow -= 10.0f;

            this->arrow->setRotation(-angleArrow + 180);

            this->frameArrow = 0;
        }
    }
    else if (this->level == 2){ // nivel 2
        if (Character::vFrames.size() == 0) return;

        if (!this->isMoving){
            setPixmap(Character::vFrames[Character::frame]);
            Character::frame = (Character::frame + 1) % Character::vFrames.size();
            setPos(Character::px, Character::py);
        }
        else {
            if (Character::direction){
                setPixmap(Character::vFramesRunningRight[Character::frame]);
                Character::frame = (Character::frame + 1) % Character::vFramesRunningRight.size();
            }
            else {
                setPixmap(Character::vFramesRunningleft[Character::frame]);
                Character::frame = (Character::frame + 1) % Character::vFramesRunningleft.size();
            }
        }

    }
}

void Player::keyPressEvent(QKeyEvent* event) {

    if (this->level == 1){ // nivel1
        if (this->isLoose) return;

        if (event->key() == Qt::Key_Space) {
            this->charging = true;

            if (!this->movingProjectile){
                this->arrow->show();
            }
        }
    }
    else { // nivel 2

        if (event->key() == Qt::Key_A){

            this->isMoving = true;
            Character::direction = false;

            if (Character::px > 7){
                Character::px = Character::px - 15;
                setPos(Character::px, Character::py);
            }
        }
        else if (event->key() == Qt::Key_D){
            Character::direction = true;
            this->isMoving = true;

            if (Character::px + boundingRect().width() <= scene->width() - 500){
                Character::px = Character::px + 15;
                setPos(Character::px, Character::py);
            }
        }
    }
}

void Player::keyReleaseEvent(QKeyEvent* event) {

    if (this->level == 1){

        if (this->isLoose) return;

        if (event->isAutoRepeat()) return;

        if (event->key() == Qt::Key_Space){

            this->arrow->hide();

            if (!this->isShooting){
                this->staticProjectile->hide();
                bool findBallShooted = false;

                if (this->movingProjectile)  findBallShooted = true;

                if (!findBallShooted){
                    this->movingProjectile = new Projectile(this, "golf", 245, 709, this->angleArrow, this->force);
                    this->scene->addItem(this->movingProjectile);
                    this->movingProjectile->setData(0, "ballShooting");
                    this->movingProjectile->initProjectile();
                    this->movingProjectile->setIsMoving(true);
                    this->hitBall->play();
                }
            }

            this->charging = false;
            this->force = 0;
        }
    }else{
        this->isMoving = false;

        if (event->isAutoRepeat()) return;

        if (event->key() == Qt::Key_Space){
            this->movingProjectile = new Projectile(this, Character::px, Character::py, 120);
            this->scene->addItem(this->movingProjectile);
            this->movingProjectile->setData(0, "ballShooting");
            this->movingProjectile->initProjectile();

            connect(this->movingProjectile, &Projectile::bossColision, this, &Player::damageToBoss);
        }
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
        this->isLoose = true;

        QGraphicsRectItem* overlay = this->scene->addRect(
            this->scene->sceneRect(),
            Qt::NoPen,
            QColor(0, 0, 0, 150)
        );

        overlay->setZValue(200);

        QGraphicsTextItem* textYouLose = this->scene->addText("Perdiste");

        QFont font("Orbitron");
        font.setBold(true);
        font.setPointSize(60);

        textYouLose->setFont(font);
        textYouLose->setDefaultTextColor(Qt::white);

        QRectF rect = this->scene->sceneRect();

        textYouLose->setPos( rect.center().x() - textYouLose->boundingRect().width()/2 - 220,
                            rect.center().y() - textYouLose->boundingRect().height()/2);

        textYouLose->setZValue(201);

        QSoundEffect* loseLife = new QSoundEffect();
        loseLife->setSource(QUrl("qrc:/audio/lose.wav"));
        loseLife->setVolume(1.0f);
        loseLife->play();

        QTimer::singleShot(3500, [=](){
            scene->removeItem(textYouLose);
            scene->removeItem(overlay);

            delete textYouLose;
            delete overlay;
            delete loseLife;

            this->attempts = 3;
            this->isLoose = false;
            for (unsigned short i = 0; i < this->hearts.size(); i++) this->hearts[i]->show();
        });

        return;
    }
}


void Player::onPortalTouched(){
    this->isLoose = true;

    if (this->movingProjectile != nullptr) {
        this->scene->removeItem(this->movingProjectile);
        delete this->movingProjectile;
        this->movingProjectile = nullptr;
        this->isShooting = false;
    }

    QGraphicsRectItem* overlay = this->scene->addRect(
        this->scene->sceneRect(),
        Qt::NoPen,
        QColor(0, 0, 0, 250)
        );

    overlay->setZValue(200);

    QGraphicsTextItem* textYouWin = this->scene->addText("Ganaste");

    QFont font("Orbitron");
    font.setBold(true);
    font.setPointSize(60);

    textYouWin->setFont(font);
    textYouWin->setDefaultTextColor(Qt::white);

    QRectF rect = this->scene->sceneRect();

    textYouWin->setPos( rect.center().x() - textYouWin->boundingRect().width()/2 - 220,
                        rect.center().y() - textYouWin->boundingRect().height()/2);

    textYouWin->setZValue(201);

    QSoundEffect* loseLife = new QSoundEffect();
    loseLife->setSource(QUrl("qrc:/audio/win.wav"));
    loseLife->setVolume(1.0f);
    loseLife->play();

    QTimer::singleShot(4500, [=](){
        scene->removeItem(textYouWin);
        scene->removeItem(overlay);

        delete textYouWin;
        delete overlay;
        delete loseLife;

        this->attempts = 3;
        this->isLoose = false;
        for (unsigned short i = 0; i < this->hearts.size(); i++) this->hearts[i]->show();


        emit levelCompleted();
    });

}

void  Player::updatePortal(){

    if (this->framePortal >= this->portals.size()) this->framePortal = 0;

    this->portal->setPixmap(this->portals[this->framePortal]);
    this->portal->setPos(1200, 600);

    this->framePortal++;
}

void  Player::getDamage(){
    loseLife();
}


Player::~Player(){
    delete this->timer;
    delete this->timerPortal;
    this->hearts.clear();
    this->portals.clear();
    delete this->hitBall;
}
