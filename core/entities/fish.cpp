#include "fish.h"

Fish::Fish(float x, float y, float energy)
    : Creature(x, y, energy, "Fish")
{
}

void Fish::update(int oceanWidth, int oceanHeight)
{
    moveBy(1.0f, 0.5f, oceanWidth, oceanHeight);
    changeEnergy(-0.3f);
}

std::shared_ptr<ICreature> Fish::clone() const
{
    return std::make_shared<Fish>(*this);
}
