#pragma once
#include <string>
#include <memory>

class IVisitor;

class ICreature {
public:
    virtual ~ICreature() = default;

    virtual void update(int oceanWidth, int oceanHeight) = 0;
    virtual void accept(IVisitor& visitor) = 0;
    virtual std::shared_ptr<ICreature> clone() const = 0;

    virtual int getId() const = 0;
    virtual void setId(int id) = 0;
    virtual float getX() const = 0;
    virtual float getY() const = 0;
    virtual float getEnergy() const = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual void setEnergy(float energy) = 0;
    virtual bool  isAlive() const = 0;
    virtual std::string getType() const = 0;
};
