#include "creature.h"
#include <utility>
#include <algorithm>

// #include "../visitors/IVisitor.h"

Creature::Creature(float x, float y, float energy, std::string type)
    : x_(x), y_(y), energy_(energy), type_(std::move(type))
{

}

void Creature::update(int oceanWidth, int oceanHeight) {
    moveBy(0.5f, 0.5f, oceanWidth, oceanHeight);
    changeEnergy(-0.05f);
}

void Creature::accept(IVisitor& visitor) {

}


std::shared_ptr<ICreature> Creature::clone() const {
    return std::make_shared<Creature>(*this);
}

void Creature::setPosition(float x, float y) {
    x_ = x;
    y_ = y;
}

void Creature::setEnergy(float energy) {
    energy_ = std::max(0.0f, energy);
}

void Creature::moveBy(float dx, float dy, int oceanWidth, int oceanHeight) {
    x_ = std::clamp(x_ + dx, 0.0f, static_cast<float>(oceanWidth));
    y_ = std::clamp(y_ + dy, 0.0f, static_cast<float>(oceanHeight));
}

void Creature::changeEnergy(float delta) {
    energy_ = std::max(0.0f, energy_ + delta);
}
