#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

class PhysicsSystem{
    public:
        PhysicsSystem();
        void mcu(float&  x, float& y, float R, float &angle);

        float getX() const;
        void setX(float newX);
        float getY() const;
        void setY(float newY);
        float getR() const;
        void setR(float newR);
        float getAngle() const;
        void setAngle(float newAngle);

    private:
        float x;
        float y;
        float R;
        float angle;


};

#endif // PHYSICSSYSTEM_H
