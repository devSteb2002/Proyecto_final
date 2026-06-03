#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->game = new Game(this);
    setCentralWidget(game);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->game;
}
