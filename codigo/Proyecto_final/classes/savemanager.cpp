#include "savemanager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

SaveManager::SaveManager() {}


void SaveManager::loadData(){
    QFile file(":/savedDATA.txt");

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
