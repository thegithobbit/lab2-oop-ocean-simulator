#pragma once
#include "ICreature.h"

class Creature : public ICreature {
public:
    Creature(float x, float y, float energy, std::string type);

    void update() override;
    void accept(IVisitor& visitor) override;

    std::shared_ptr<ICreature> clone() const override;

    float getX()      const override { return x_; }
    float getY()      const override { return y_; }
    float getEnergy() const override { return energy_; }
    bool  isAlive()   const override { return energy_ > 0.0f; }
    std::string getType() const override { return type_; }

protected:
    float x_, y_;
    float energy_;
    std::string type_;
};
