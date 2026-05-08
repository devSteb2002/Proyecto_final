#include "character.h"

Character::Character() {}

Character::Character(PhysicsSystem* physics): physics(physics){

}


Character::~Character(){
    if (this->physics != nullptr){
        delete this->physics;
    }
}
