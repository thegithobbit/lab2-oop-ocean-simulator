#ifndef OCEAN_H
#define OCEAN_H

#include <QObject>
#include <vector>
#include <memory>
#include "entities/ICreature.h"

class Ocean : public QObject {
    Q_OBJECT
public:
    explicit Ocean(QObject *parent = nullptr) : QObject(parent) {}

    void addCreature(std::shared_ptr<ICreature> creature) {
        creatures.push_back(creature);
    }

    void updateAll() {
        for (auto& creature : creatures) {
            creature->update();
        }
    }

private:
    std::vector<std::shared_ptr<ICreature>> creatures;
};

#endif // OCEAN_H
