#pragma once

#include "creature.h"

class Shark : public Creature {
public:
    Shark(float x, float y, float energy);

    void update(int oceanWidth, int oceanHeight) override;
    std::shared_ptr<ICreature> clone() const override;
};
