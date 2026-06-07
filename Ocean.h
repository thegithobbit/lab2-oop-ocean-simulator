#ifndef OCEAN_H
#define OCEAN_H

#include <QObject>
#include <vector>
#include <memory>
#include <QString>
#include "entities/ICreature.h"

struct OceanStats {
    int total = 0;
    int alive = 0;
    int fish = 0;
    int sharks = 0;
    int plankton = 0;
    float averageEnergy = 0.0f;
};

class Ocean : public QObject {
    Q_OBJECT
public:
    explicit Ocean(QObject *parent = nullptr);

    const std::vector<std::shared_ptr<ICreature>>& getCreatures() const;

    void addCreature(std::shared_ptr<ICreature> creature);
    bool updateCreature(int id, const QString& type, float x, float y, float energy);
    bool removeCreature(int id);
    void clear();
    void step();
    void seedDemoData();

    int width() const;
    int height() const;
    void setSize(int width, int height);
    OceanStats stats() const;

signals:
    void changed();

private:
    std::shared_ptr<ICreature> createCreature(const QString& type, float x, float y, float energy) const;

    std::vector<std::shared_ptr<ICreature>> creatures_;
    int nextId_ = 1;
    int width_ = 100;
    int height_ = 60;
};

#endif // OCEAN_H
