#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QFontDatabase>
#include <QFile>
#include "classes/player.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFontDatabase::addApplicationFont(":/fonts/Orbitron-Medium.ttf");

    this->stack = new QStackedWidget(this); //gestor de "escenas"
    this->menuScreen = new QWidget();
    this->menuScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->bgMenu = new QLabel(menuScreen);
    this->audioOutput = new QAudioOutput(this); // salida del audio
    this->theme = new QMediaPlayer(this); // tema principal

    QVBoxLayout* menuLayout = new QVBoxLayout(menuScreen);
    QLabel* title = new QLabel("Golfito");
    QPushButton* playBtn = new QPushButton("Jugar");
    QPushButton* exitBtn = new QPushButton("Salir");

    menuLayout->setContentsMargins(0, 0, 0, 0);

   QPixmap sheet(":/images/backgorund.jpg");
   this->originalFrame = sheet;

   bgMenu->lower();

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

   QTimer::singleShot(0, this, [=]() {
       QPixmap scaled = originalFrame.scaled(
           menuScreen->size(),
           Qt::KeepAspectRatioByExpanding,
           Qt::SmoothTransformation
           );

       this->bgMenu->setAlignment(Qt::AlignCenter);
       this->bgMenu->setPixmap(scaled);
       this->bgMenu->setGeometry(menuScreen->rect());

   });

   //---------------------------------------------------------------
   // Nivel 1
   //---------------------------------------------------------------

   this->scene = new QGraphicsScene(this);
   this->view = new QGraphicsView(scene, this);
   this->stack->addWidget(view);
   this->scene->setSceneRect(0, 0, 2000, 700);

   this->view->setSceneRect(this->scene->sceneRect());
   this->view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
   this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   this->view->setFrameShape(QFrame::NoFrame);
   this->view->setBackgroundBrush(Qt::NoBrush);

   //nivel 1
   // QGraphicsRectItem* ground = new QGraphicsRectItem(0, 500, 2000, 100);
   // ground->setBrush(Qt::darkGreen);
   // scene->addItem(ground);

   qDebug() << "view rect:" << view->rect();
   qDebug() << "scene rect:" << scene->sceneRect();

   QPixmap bg(":/images/bg-level-1.png");

   bg = bg.scaled(scene->sceneRect().width(),
                  scene->sceneRect().height(),
                  Qt::IgnoreAspectRatio,
                  Qt::SmoothTransformation);

   QGraphicsPixmapItem* background = new QGraphicsPixmapItem(bg);

   background->setPos(0, 0);
   background->setZValue(-1000);

   scene->addItem(background);

   Player* player = new Player();

   this->scene->addItem(player);

}



MainWindow::~MainWindow()
{
    delete ui;
    delete stack;
    delete menuScreen;
}
