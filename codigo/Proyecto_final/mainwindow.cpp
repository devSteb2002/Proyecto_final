#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QFile>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont(":/fonts/Orbitron-Medium.ttf");

    if (this->layout()){
        this->layout()->setContentsMargins(0, 0, 0, 0);
        this->layout()->setSpacing(0);
    }

    if (this->centralWidget()) this->centralWidget()->setContentsMargins(0, 0, 0, 0);

    this->stack           = new QStackedWidget(this); //gestor de "escenas"
    this->menuScreen = new QWidget();
    this->bgMenu       = new QLabel(menuScreen);
    this->audioOutput  = new QAudioOutput(this); // salida del audio
    this->theme          = new QMediaPlayer(this); // tema principal

    this->menuScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->stack->setFrameShape(QFrame::NoFrame);
    this->stack->setFrameStyle(QFrame::NoFrame);
    this->stack->setLineWidth(0);
    this->stack->setMidLineWidth(0);
    this->stack->setContentsMargins(0, 0, 0, 0);

    this->centralWidget()->setStyleSheet("margin: 0px; padding: 0px; border: none;");

    QVBoxLayout* menuLayout   = new QVBoxLayout(menuScreen);
    QLabel* title                         = new QLabel("Golfito");
    QPushButton* playBtn           = new QPushButton("Jugar");
    QPushButton* exitBtn           = new QPushButton("Salir");

    menuLayout->setContentsMargins(0, 0, 0, 0);

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
   setCentralWidget(stack);

  this->showFullScreen();

  //configuracion del audio
  this->theme->setAudioOutput(this->audioOutput);
  this->theme->setSource(QUrl("qrc:/audio/principal-theme.mp3"));
  this->audioOutput->setVolume(0.5);
  this->theme->play();
  this->theme->setLoops(QMediaPlayer::Infinite);

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

   });

   //---------------------------------------------------------------
   // Nivel 1
   //---------------------------------------------------------------

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


  this->physics = new PhysicsSystem();
  this->player = new Player(this->scene);

   connect(playBtn, &QPushButton::clicked, this, [=](){
       this->stack->setCurrentIndex(1);
       this->theme->stop();

       this->designLevel1(scene); // diseño nivel 1

       this->scene->addItem(player);
       this->player->initPlayer();


       //enemigos
       Enemy* enemy1 = new Enemy(true, "fire", 0, 573, false, true);
       // Enemy* enemy2 = new Enemy(true, "fire", 480, 100, true, false);
       // Enemy* enemy3 = new Enemy(true, "fire", 540, 100, true, false);
      // Enemy* robot1  = new Enemy(physics, "robot", 500, 500, true);

       this->scene->addItem(enemy1);
       // this->scene->addItem(enemy2);
       // this->scene->addItem(enemy3);
      // this->scene->addItem(robot1);

       enemy1->intiEnemy();
       // enemy2->intiEnemy();
       // enemy3->intiEnemy();
       //robot1->intiEnemy();

       // this->enemies.push_back(enemy1);
       // this->enemies.push_back(enemy2);
       // this->enemies.push_back(enemy3);
       //this->enemies.push_back(robot1);

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
       close();
   });


}

void MainWindow::designLevel1(QGraphicsScene*& scene){
    QPixmap bg(":/images/bg-level-1.png");
    QPixmap walls(":/images/textures.png");
    QPixmap assets(":/images/sonic-scrap_.png");

    bg = bg.scaled(scene->sceneRect().width(), scene->sceneRect().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(bg);
    QGraphicsPixmapItem* walls_         = nullptr;
    QGraphicsPixmapItem* rotateWalls_= nullptr;

    QPixmap platform            = assets.copy(470, 587, 200, 50).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap platformRotate = assets.copy(470, 587, 200, 50).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(180));

    walls_          = new QGraphicsPixmapItem(platform);
    rotateWalls_ = new QGraphicsPixmapItem(platformRotate);

    walls_->setPos(0, 750);
    walls_->setData(0, "wall");
    rotateWalls_->setPos(188, 753.7);
    rotateWalls_->setData(0, "wall");

    background->setPos(0, 0);
    background->setZValue(-1000);

    scene->addItem(background);
    scene->addItem(walls_);
    scene->addItem(rotateWalls_);
}


void MainWindow::designLevel2(QGraphicsScene*& scene){

}


MainWindow::~MainWindow()
{

    this->scene->clear();
    this->enemies.clear();
    delete this->physics;

    delete ui;
}
