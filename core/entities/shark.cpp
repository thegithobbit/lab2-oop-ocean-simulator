#include "shark.h"

Shark::Shark(float x, float y, float energy)
    : Creature(x, y, energy, "Shark")
{
}

void Shark::update(int oceanWidth, int oceanHeight)
{
    moveBy(-1.4f, 0.8f, oceanWidth, oceanHeight);
    changeEnergy(-0.6f);
}

std::shared_ptr<ICreature> Shark::clone() const
{
    return std::make_shared<Shark>(*this);
}
