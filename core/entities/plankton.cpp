#include "plankton.h"

Plankton::Plankton(float x, float y, float energy)
    : Creature(x, y, energy, "Plankton")
{
}

void Plankton::update(int oceanWidth, int oceanHeight)
{
    moveBy(0.2f, 0.1f, oceanWidth, oceanHeight);
    changeEnergy(0.1f);
}

std::shared_ptr<ICreature> Plankton::clone() const
{
    return std::make_shared<Plankton>(*this);
}
