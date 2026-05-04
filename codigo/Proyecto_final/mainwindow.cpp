#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFontDatabase>
#include <QFile>
#include <QScreen>
#include "classes/player.h"

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

   connect(playBtn, &QPushButton::clicked, this, [=](){
         this->stack->setCurrentIndex(1);
         this->theme->stop();
   });

   connect(exitBtn, &QPushButton::clicked, this, [=](){
       close();
   });

   QTimer::singleShot(50, this, [=]() {

       // 1. Obtenemos el rectángulo real del contenedor
       QRect rectCapa = menuScreen->rect();

       // 2. Le damos un pequeño margen de error para que no se vea el fondo
       rectCapa.setWidth(rectCapa.width() + 2);
       rectCapa.setHeight(rectCapa.height() + 2);

       // 3. Escalamos la imagen basándonos en ese nuevo tamaño
       QPixmap scaled = originalFrame.scaled(
           rectCapa.size(),
           Qt::KeepAspectRatioByExpanding,
           Qt::SmoothTransformation
           );

       // 4. Aplicamos todo al Label en un solo paso
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

   this->designLevel1(scene); // diseño nivel 1

   Player* player = new Player();
   this->scene->addItem(player);

   QTimer::singleShot(50, this, [=]() {
       QRect screenRect = this->screen()->geometry();

        this->setFixedSize(screenRect.size());
        this->stack->setFixedSize(screenRect.size());
        this->scene->setSceneRect(0, 0, 2000, screenRect.height());
        this->view->setFixedSize(screenRect.size());
        this->view->setStyleSheet("background-color: #040405; border: none;");
   });

}

void MainWindow::designLevel1(QGraphicsScene*& scene){
    QPixmap bg(":/images/bg-level-1.png");

    bg = bg.scaled(scene->sceneRect().width(),
                   scene->sceneRect().height(),
                   Qt::IgnoreAspectRatio,
                   Qt::SmoothTransformation);

    unsigned const short   des = 50;
    QGraphicsPixmapItem* background = new QGraphicsPixmapItem(bg);

    QPixmap walls(":/images/walls.png");
    QPixmap stoneWall = walls.copy(234, 263, 30, 20).scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    const unsigned short px = 75;
    unsigned short py = 840;

    for (unsigned short i = 0; i < 6; i++){
        QGraphicsPixmapItem* walls_   = new QGraphicsPixmapItem(stoneWall);
        QGraphicsPixmapItem* walls_2   = new QGraphicsPixmapItem(stoneWall);
        QGraphicsPixmapItem* walls_3   = new QGraphicsPixmapItem(stoneWall);
        QGraphicsPixmapItem* walls_4   = new QGraphicsPixmapItem(stoneWall);

        walls_->setPos(0, py);
        walls_->setData(0, "wall");
        scene->addItem(walls_);

        walls_2->setPos(px, py);
        walls_->setData(0, "wall");
        scene->addItem(walls_2);

        walls_3->setPos(px + 75, py);
        walls_->setData(0, "wall");
        scene->addItem(walls_3);

        walls_4->setPos(px + 150, py);
        walls_->setData(0, "wall");
        scene->addItem(walls_4);

        py -= 40;
    }

    QPixmap leafWall    = walls.copy(148, 120, 20, 20).scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QGraphicsPixmapItem* walls_   = new QGraphicsPixmapItem(leafWall);
    walls_->setPos(-2, py - 6);
    scene->addItem(walls_);



    background->setPos(0, 0);
    background->setZValue(-1000);

    scene->addItem(background);

}


void MainWindow::designLevel2(QGraphicsScene*& scene){

}


MainWindow::~MainWindow()
{
    delete ui;
    delete stack;
    delete menuScreen;
}
