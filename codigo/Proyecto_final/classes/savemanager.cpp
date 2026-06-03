#include "savemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

SaveManager::SaveManager() {}

void SaveManager::loadData(){

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString filePath = path + "/savedDATA.txt";

    QFile file(filePath);

    if (!file.exists()){
        qDebug() << "No exite el archivo";

        this->level = 1;
        this->learningEnemy = 1;

        this->saveData();

        return;
    }

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);

            QString contenido = in.readAll();
            QString level = contenido[0];
            QString learning = contenido[2];

            unsigned short level_ = level.toShort();
            unsigned short learning_ = learning.toShort();

            this->learningEnemy = learning_;
            this->level = level_;

            file.close();
            qDebug() << "Nivel:" << level;
            qDebug() << "Learning:" << learningEnemy;
    }
    else {
        qDebug() << "Error al leer:";
        qDebug() << file.errorString();
    }
}

void SaveManager::saveData(){
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir().mkpath(path);

    QString filePath = path + "/savedDATA.txt";

    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        out << this->getLevel()
            << ";"
            << this->getLearningEnemy();

        file.close();

        qDebug() << "Datos guardados en:";
        qDebug() << filePath;
    }
    else
    {
        qDebug() << "Error al guardar:";
        qDebug() << file.errorString();
    }
}

unsigned short SaveManager::getLevel() const
{
    return level;
}

void SaveManager::setLevel(unsigned short newLevel)
{
    level = newLevel;
}

unsigned short SaveManager::getLearningEnemy() const
{
    return learningEnemy;
}

void SaveManager::setLearningEnemy(unsigned short newLearningEnemy)
{
    learningEnemy = newLearningEnemy;
}

SaveManager::~SaveManager(){

}
