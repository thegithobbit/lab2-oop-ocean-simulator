#include "Ocean.h"
#include "core/entities/fish.h"
#include "core/entities/shark.h"
#include "core/entities/plankton.h"

#include <algorithm>
#include <numeric>

Ocean::Ocean(QObject *parent)
    : QObject(parent)
{
    seedDemoData();
}

const std::vector<std::shared_ptr<ICreature>>& Ocean::getCreatures() const
{
    return creatures_;
}

void Ocean::addCreature(std::shared_ptr<ICreature> creature)
{
    creature->setId(nextId_++);
    creatures_.push_back(std::move(creature));
    emit changed();
}

bool Ocean::updateCreature(int id, const QString& type, float x, float y, float energy)
{
    auto it = std::find_if(creatures_.begin(), creatures_.end(), [id](const auto& creature) {
        return creature->getId() == id;
    });

    if (it == creatures_.end()) {
        return false;
    }

    auto updated = createCreature(type, x, y, energy);
    updated->setId(id);
    *it = updated;
    emit changed();
    return true;
}

bool Ocean::removeCreature(int id)
{
    const auto oldSize = creatures_.size();
    creatures_.erase(std::remove_if(creatures_.begin(), creatures_.end(), [id](const auto& creature) {
        return creature->getId() == id;
    }), creatures_.end());

    const bool removed = creatures_.size() != oldSize;
    if (removed) {
        emit changed();
    }
    return removed;
}

void Ocean::clear()
{
    creatures_.clear();
    emit changed();
}

void Ocean::step()
{
    for (auto& creature : creatures_) {
        creature->update(width_, height_);
    }
    emit changed();
}

void Ocean::seedDemoData()
{
    creatures_.clear();
    nextId_ = 1;
    addCreature(std::make_shared<Fish>(12, 14, 80));
    addCreature(std::make_shared<Fish>(30, 20, 65));
    addCreature(std::make_shared<Shark>(70, 35, 95));
    addCreature(std::make_shared<Plankton>(44, 10, 30));
}

int Ocean::width() const
{
    return width_;
}

int Ocean::height() const
{
    return height_;
}

void Ocean::setSize(int width, int height)
{
    width_ = std::max(10, width);
    height_ = std::max(10, height);
    emit changed();
}

OceanStats Ocean::stats() const
{
    OceanStats result;
    result.total = static_cast<int>(creatures_.size());

    float energySum = 0.0f;
    for (const auto& creature : creatures_) {
        if (creature->isAlive()) {
            ++result.alive;
        }

        const auto type = creature->getType();
        if (type == "Fish") {
            ++result.fish;
        } else if (type == "Shark") {
            ++result.sharks;
        } else if (type == "Plankton") {
            ++result.plankton;
        }

        energySum += creature->getEnergy();
    }

    if (result.total > 0) {
        result.averageEnergy = energySum / result.total;
    }
    return result;
}

std::shared_ptr<ICreature> Ocean::createCreature(const QString& type, float x, float y, float energy) const
{
    if (type == "Shark") {
        return std::make_shared<Shark>(x, y, energy);
    }
    if (type == "Plankton") {
        return std::make_shared<Plankton>(x, y, energy);
    }
    return std::make_shared<Fish>(x, y, energy);
}
