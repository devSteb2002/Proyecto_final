#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

class SaveManager {
    public:
        SaveManager();
        void loadData();
        void saveData();

        unsigned short getLevel() const;
        void setLevel(unsigned short newLevel);

        unsigned short getLearningEnemy() const;
        void setLearningEnemy(unsigned short newLearningEnemy);

        ~SaveManager();

    private:
        unsigned short level;
        unsigned short learningEnemy;
};

#endif // SAVEMANAGER_H
