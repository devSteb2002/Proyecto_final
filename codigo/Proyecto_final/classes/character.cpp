#include "character.h"

Character::Character() {}
Character::Character(PhysicsSystem* physics): physics(physics){}

void Character::reinitBall(){}
void Character::loseLife(){}
void Character::onPortalTouched(){}

Character::~Character(){
    if (this->physics != nullptr){
        delete this->physics;
    }
}
