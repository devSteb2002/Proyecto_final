#include "physicssystem.h"
#include <cmath>

PhysicsSystem::PhysicsSystem() {}

void PhysicsSystem::mcu(float&  x, float& y, float R, float &angle){
    x = 500 + R * cos(angle);
    y = 500 + R * sin(angle);
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
