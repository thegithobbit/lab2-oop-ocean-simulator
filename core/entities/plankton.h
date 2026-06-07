#pragma once

#include "creature.h"

class Plankton : public Creature {
public:
    Plankton(float x, float y, float energy);

    void update(int oceanWidth, int oceanHeight) override;
    std::shared_ptr<ICreature> clone() const override;
};
