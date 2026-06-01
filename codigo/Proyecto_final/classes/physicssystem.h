#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

class PhysicsSystem{
    public:
        PhysicsSystem();
        void mcu(float&  x, float& y, float R, float &angle);
        void parabolicMotion(float& x, float& y, float angle, float time, float v0);
        void freeFallMotion(float& y, float y0, float v0, float time);

        float getX() const;
        void setX(float newX);
        float getY() const;
        void setY(float newY);
        float getR() const;
        void setR(float newR);
        float getAngle() const;
        void setAngle(float newAngle);

        float getX0() const;
        float getY0() const;

        void setX0(float newX0);
        void setY0(float newY0);

    private:
        float x;
        float y;
        float R;
        float angle;
        float x0;
        float y0;


};

#endif // PHYSICSSYSTEM_H
