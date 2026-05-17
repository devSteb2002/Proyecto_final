#include "physicssystem.h"
#include <cmath>

PhysicsSystem::PhysicsSystem() {}

void PhysicsSystem::mcu(float&  x, float& y, float R, float &angle){
    x = x + R * cos(angle);
    y = y + R * sin(angle);
}

void PhysicsSystem::parabolicMotion(float& x, float& y, float angle, float time, float v0){
    float radians = angle * M_PI / 180.0f;
    float vx =v0 * cos(radians);
    float vy = v0 * sin(radians);

    x = x0 + vx * time;

    y = y0 -(vy * time - 0.5f * 9.8f * time * time);
}

float PhysicsSystem::getX() const
{
    return x;
}

void PhysicsSystem::setX(float newX)
{
    x = newX;
}

float PhysicsSystem::getY() const
{
    return y;
}

void PhysicsSystem::setY(float newY)
{
    y = newY;
}

float PhysicsSystem::getR() const
{
    return R;
}

void PhysicsSystem::setR(float newR)
{
    R = newR;
}

float PhysicsSystem::getAngle() const
{
    return angle;
}

void PhysicsSystem::setAngle(float newAngle)
{
    angle = newAngle;
}

float PhysicsSystem::getX0() const
{
    return x0;
}

float PhysicsSystem::getY0() const
{
    return y0;
}

void PhysicsSystem::setX0(float newX0)
{
    x0 = newX0;
}

void PhysicsSystem::setY0(float newY0)
{
    y0 = newY0;
}
