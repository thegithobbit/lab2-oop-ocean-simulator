#pragma once

#include "creature.h"

class Fish : public Creature {
public:
    Fish(float x, float y, float energy);

    void update(int oceanWidth, int oceanHeight) override;
    std::shared_ptr<ICreature> clone() const override;
};
