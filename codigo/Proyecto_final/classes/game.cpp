#include "game.h"
#include <QFontDatabase>
#include <QFile>
#include <QScreen>

Game::Game() {}

Game::Game(QWidget* parent): QWidget(parent){

    //------------- Cargar datos --------------------------//
    this->savemanager = new SaveManager();
    this->savemanager->loadData();

    // ---------------- Generar menu de inicio ------------//
    QFontDatabase::addApplicationFont(":/fonts/Orbitron-Medium.ttf");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    if (this->layout()){
        this->layout()->setContentsMargins(0, 0, 0, 0);
        this->layout()->setSpacing(0);
    }

    this->stack           = new QStackedWidget(this); //gestor de "escenas"
    this->menuScreen = new QWidget();
    this->bgMenu       = new QLabel(menuScreen);

    this->menuScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->stack->setFrameShape(QFrame::NoFrame);
    this->stack->setFrameStyle(QFrame::NoFrame);
    this->stack->setLineWidth(0);
    this->stack->setMidLineWidth(0);
    this->stack->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(this->stack);

    QVBoxLayout* menuLayout   = new QVBoxLayout(menuScreen);
    menuLayout->setContentsMargins(0, 0, 0, 0);
    menuLayout->setSpacing(15);

    QLabel* title                         = new QLabel("Golfito");
    QPushButton* playBtn           = new QPushButton("Jugar");
    QPushButton* exitBtn           = new QPushButton("Salir");

    QPixmap sheet(":/images/backgorund.jpg");

    this->originalFrame = sheet;
    this->bgMenu->lower();

    playBtn->setFixedWidth(300);
    playBtn->setCursor(Qt::PointingHandCursor);
    menuLayout->setSpacing(15);
    exitBtn->setFixedWidth(300);
    exitBtn->setCursor(Qt::PointingHandCursor);

    title->setStyleSheet(
        "color: white;"
        "font-size: 78px;"
        "font-weight: bold;"
        "font-family: 'Orbitron';"
        );

    playBtn->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0, 0, 0, 150);"
        "color: white;"
        "font-family: 'Orbitron';"
        "font-size: 24px;"
        "border: 2px solid white;"
        "border-radius: 10px;"
        "padding: 10px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 255, 255, 50);"
        "color: black;"
        "}"

        "QPushButton:pressed {"
        "background-color: rgba(255, 255, 255, 100);"
        "}"
        );

    exitBtn->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(0, 0, 0, 150);"
        "color: white;"
        "font-size: 24px;"
        "border: 2px solid white;"
        "font-family: 'Orbitron';"
        "border-radius: 10px;"
        "padding: 10px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 255, 255, 50);"
        "color: black;"
        "}"

        "QPushButton:pressed {"
        "background-color: rgba(255, 255, 255, 100);"
        "}"
        );

    menuLayout->addStretch();
    menuLayout->addWidget(title, 0, Qt::AlignCenter);
    menuLayout->addSpacing(100);
    menuLayout->addWidget(playBtn, 0 , Qt::AlignCenter);
    menuLayout->addSpacing(50);
    menuLayout->addWidget(exitBtn,  0 , Qt::AlignCenter);
    menuLayout->addStretch();

    this->stack->addWidget(menuScreen);

    this->showFullScreen();

    //configuracion del audio
    this->theme = new QSoundEffect(this);
    this->theme->setSource(QUrl("qrc:/audio/principal-theme.wav"));
    this->theme->setVolume(0.5f);
    this->theme->setLoopCount(QSoundEffect::Infinite);
    this->theme->play();

    QTimer::singleShot(50, this, [=]() {

        QRect rectCapa = menuScreen->rect();

        rectCapa.setWidth(rectCapa.width() + 2);
        rectCapa.setHeight(rectCapa.height() + 2);

        QPixmap scaled = originalFrame.scaled(
            rectCapa.size(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation
            );

        this->bgMenu->setPixmap(scaled);
        this->bgMenu->setGeometry(rectCapa);
        this->bgMenu->setAlignment(Qt::AlignCenter);

         qDebug() << menuScreen->size();
    });


    this->scene = new QGraphicsScene(this);
    this->view   = new QGraphicsView(scene, this);

    this->stack->addWidget(view);
    this->scene->setSceneRect(0, 0, 2000, 600);

    this->view->setSceneRect(this->scene->sceneRect());
    this->view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setFrameShape(QFrame::NoFrame);
    this->view->setBackgroundBrush(Qt::NoBrush);
    this->view->setStyleSheet("background: transparent; border: none;");

    this->savemanager->setLevel(1);
    this->physics = new PhysicsSystem();
    this->player = new Player(this->scene, this->savemanager->getLevel());

    connect(this->player, &Player::levelCompleted, this, &Game::updateLevel);

    connect(playBtn, &QPushButton::clicked, this, [=](){

        if (this->savemanager->getLevel() == 1){

            this->stack->setCurrentIndex(1);
            this->theme->stop();

            this->designLevel1(scene); // diseño nivel 1

            this->scene->addItem(player);
            this->player->initPlayer();

            //enemigos
            Enemy* enemy1 = new Enemy(true, "fire", 0, 573, false, true);
            this->scene->addItem(enemy1);
            enemy1->intiEnemy();

        }
        else{
            this->stack->setCurrentIndex(1);
            this->designLevel2(scene);
            this->theme->stop();

            this->scene->addItem(this->player);
            this->player->initPlayer2();
            this->player->setData(0, "player");
        }

        QTimer::singleShot(50, this, [=]() {
            QRect screenRect = this->screen()->geometry();

            this->setFixedSize(screenRect.size());
            this->stack->setFixedSize(screenRect.size());
            this->scene->setSceneRect(0, 0, 2000, screenRect.height());
            this->view->setFixedSize(screenRect.size());
            this->view->setStyleSheet("background-color: #000018; border: none;");
        });
    });

    connect(exitBtn, &QPushButton::clicked, this, [=](){
        qApp->quit();
    });
}

void Game::designLevel1(QGraphicsScene*& scene){
    QPixmap bg(":/images/bg-level-1.png");
    QPixmap assets(":/images/sonic-scrap_.png");

    bg = bg.scaled(scene->sceneRect().width(), scene->sceneRect().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(bg);
    QGraphicsPixmapItem* walls_         = nullptr;
    QGraphicsPixmapItem* rotateWalls_= nullptr;
    QGraphicsPixmapItem* platformw_    = nullptr;
    QGraphicsPixmapItem* platformw_2    = nullptr;
    QGraphicsPixmapItem* platformw_3    = nullptr;

    QPixmap platform            = assets.copy(470, 587, 200, 50).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap platformRotate = assets.copy(470, 587, 200, 50).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(180));
    QPixmap platformCooli    = assets.copy(262, 270, 55, 20).scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(270));


    walls_          = new QGraphicsPixmapItem(platform);
    rotateWalls_ = new QGraphicsPixmapItem(platformRotate);
    platformw_ = new QGraphicsPixmapItem(platformCooli);
    platformw_2 = new QGraphicsPixmapItem(platformCooli);
    platformw_3 = new QGraphicsPixmapItem(platformCooli);

    walls_->setPos(0, 750);
    walls_->setData(0, "wall");
    rotateWalls_->setPos(188, 753.7);
    rotateWalls_->setData(0, "wall");

    platformw_->setPos(600, 400);
    platformw_2->setPos(800,700);
    platformw_->setData(0, "platform");
    platformw_2->setData(0, "platform");
    platformw_3->setPos(1200, 200);
    platformw_3->setData(0, "platform");

    background->setPos(0, 0);
    background->setZValue(-1000);

    scene->addItem(background);
    scene->addItem(walls_);
    scene->addItem(rotateWalls_);
    scene->addItem(platformw_);
    scene->addItem(platformw_2);
    scene->addItem(platformw_3);
}


void Game::designLevel2(QGraphicsScene*& scene){
    QPixmap bg(":/images/bg-level-1.png");
    QPixmap assets(":/images/sonic-scrap_.png");

    QPixmap platform            = assets.copy(470, 587, 200, 50).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    bg = bg.scaled(scene->sceneRect().width(), scene->sceneRect().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* walls_           = nullptr;
    QGraphicsPixmapItem* walls_1         = nullptr;
    QGraphicsPixmapItem* walls_2         = nullptr;
    QGraphicsPixmapItem* walls_3         = nullptr;
    QGraphicsPixmapItem* walls_4         = nullptr;
    QGraphicsPixmapItem* walls_5         = nullptr;
    QGraphicsPixmapItem* walls_6         = nullptr;
    QGraphicsPixmapItem* walls_7         = nullptr;
    QGraphicsPixmapItem* walls_8         = nullptr;

    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(bg);

    walls_           = new QGraphicsPixmapItem(platform);
    walls_1         = new QGraphicsPixmapItem(platform);
    walls_2         = new QGraphicsPixmapItem(platform);
    walls_3         = new QGraphicsPixmapItem(platform);
    walls_4         = new QGraphicsPixmapItem(platform);
    walls_5         = new QGraphicsPixmapItem(platform);
    walls_6         = new QGraphicsPixmapItem(platform);
    walls_7         = new QGraphicsPixmapItem(platform);
    walls_8         = new QGraphicsPixmapItem(platform);

    walls_->setPos(0, 850);
    walls_->setData(0, "wall");
    walls_1->setPos(188, 850);
    walls_1->setData(0, "wall");
    walls_2->setPos(376, 850);
    walls_2->setData(0, "wall");
    walls_3->setPos(564, 850);
    walls_3->setData(0, "wall");
    walls_4->setPos(752, 850);
    walls_4->setData(0, "wall");
    walls_5->setPos(940, 850);
    walls_5->setData(0, "wall");
    walls_6->setPos(1128, 850);
    walls_6->setData(0, "wall");
    walls_7->setPos(1316, 850);
    walls_7->setData(0, "wall");
    walls_8->setPos(1504, 850);
    walls_8->setData(0, "wall");

    background->setPos(0, 0);
    background->setZValue(-1000);

    scene->addItem(background);
    scene->addItem(walls_);
    scene->addItem(walls_1);
    scene->addItem(walls_2);
    scene->addItem(walls_3);
    scene->addItem(walls_4);
    scene->addItem(walls_5);
    scene->addItem(walls_6);
    scene->addItem(walls_7);
    scene->addItem(walls_8);

    Enemy* robot1  = new Enemy(physics, "robot", 100, 100, true, scene);
    Enemy* robot2  = new Enemy(physics, "robot", 1350, 100, true, scene);
    Enemy* robot3  = new Enemy(physics, "robot", 400, 100, true, scene);
    Enemy* robot4  = new Enemy(physics, "robot", 1000, 100, true, scene);

    scene->addItem(robot1);
    scene->addItem(robot2);
    scene->addItem(robot3);
    scene->addItem(robot4);
    robot1->setData(0, "boss");
    robot2->setData(0, "boss");
    robot3->setData(0, "boss");
    robot4->setData(0, "boss");
    robot1->intiEnemy();
    robot2->intiEnemy();
    robot3->intiEnemy();
    robot4->intiEnemy();

    this->enemies = {
        robot1,
        robot2,
        robot3,
        robot4
    };

    connect(robot1, &Enemy::damageToPlayer, this->player, &Player::getDamage);
    connect(robot2, &Enemy::damageToPlayer, this->player, &Player::getDamage);
    connect(robot3, &Enemy::damageToPlayer, this->player, &Player::getDamage);
    connect(robot4, &Enemy::damageToPlayer, this->player, &Player::getDamage);

    connect(this->player, &Player::damageToBoss, robot1, &Enemy::getDamage);
    connect(this->player, &Player::damageToBoss, robot2, &Enemy::getDamage);
    connect(this->player, &Player::damageToBoss, robot3, &Enemy::getDamage);
    connect(this->player, &Player::damageToBoss, robot4, &Enemy::getDamage);
}


void Game::updateLevel(){
    if (this->savemanager->getLevel() == 1) this->savemanager->setLevel(2);
    else this->savemanager->setLevel(1);

    if (!this->scene->items().isEmpty()) {
        this->scene->clear();
        this->player = nullptr;
        this->player = new Player(this->scene, this->savemanager->getLevel());
    }

    if (this->savemanager->getLevel() == 2){
        this->designLevel2(this->scene);
    }

    qDebug() << "cambio de nivel";
}

Game::~Game(){

    this->scene->clear();
    this->enemies.clear();
    delete this->physics;
    delete this->savemanager;
    delete this->theme;
}
