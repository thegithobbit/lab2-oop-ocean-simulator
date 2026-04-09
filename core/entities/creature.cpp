#include "Creature.h"
#include <utility>

// #include "../visitors/IVisitor.h"

Creature::Creature(float x, float y, float energy, std::string type)
    : x_(x), y_(y), energy_(energy), type_(std::move(type))
{

}

void Creature::update() {
    if (energy_ > 0) {
        energy_ -= 0.05f;
    }
}

void Creature::accept(IVisitor& visitor) {

}


std::shared_ptr<ICreature> Creature::clone() const {
    return std::make_shared<Creature>(*this);
}
