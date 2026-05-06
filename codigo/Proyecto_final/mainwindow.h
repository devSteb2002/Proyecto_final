#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QStackedWidget* stack;
    QWidget*            menuScreen;
    QPixmap             originalFrame;
    QLabel*              bgMenu;
    QAudioOutput*    audioOutput;
    QMediaPlayer*     theme;
    QGraphicsScene* scene;
    QGraphicsView*   view;
    QGraphicsView*   view2;

    void designLevel1(QGraphicsScene*& scene);
    void designLevel2(QGraphicsScene*& scene);
    void updateLevel();

};
#endif // MAINWINDOW_H
