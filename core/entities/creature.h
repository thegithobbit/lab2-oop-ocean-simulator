#pragma once
#include "ICreature.h"

class Creature : public ICreature {
public:
    Creature(float x, float y, float energy, std::string type);

    void update(int oceanWidth, int oceanHeight) override;
    void accept(IVisitor& visitor) override;

    std::shared_ptr<ICreature> clone() const override;

    int getId() const override { return id_; }
    void setId(int id) override { id_ = id; }
    float getX()      const override { return x_; }
    float getY()      const override { return y_; }
    float getEnergy() const override { return energy_; }
    void setPosition(float x, float y) override;
    void setEnergy(float energy) override;
    bool  isAlive()   const override { return energy_ > 0.0f; }
    std::string getType() const override { return type_; }

protected:
    void moveBy(float dx, float dy, int oceanWidth, int oceanHeight);
    void changeEnergy(float delta);

    int id_ = 0;
    float x_, y_;
    float energy_;
    std::string type_;
};
