#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsPixmapItem>

class Character : public QGraphicsPixmapItem
{
    public:
        Character();

    protected:
        unsigned short attempts;
        unsigned short force;


};

#endif // CHARACTER_H
