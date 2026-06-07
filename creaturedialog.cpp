#include "creaturedialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QVBoxLayout>

CreatureDialog::CreatureDialog(QWidget *parent)
    : QDialog(parent),
      typeCombo_(new QComboBox(this)),
      xSpin_(new QDoubleSpinBox(this)),
      ySpin_(new QDoubleSpinBox(this)),
      energySpin_(new QDoubleSpinBox(this)),
      buttons_(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    setWindowTitle("Creature");

    typeCombo_->addItems({"Fish", "Shark", "Plankton"});

    for (auto spin : {xSpin_, ySpin_}) {
        spin->setRange(0.0, 100.0);
        spin->setDecimals(1);
    }

    energySpin_->setRange(0.0, 100.0);
    energySpin_->setDecimals(1);
    energySpin_->setValue(50.0);

    auto *form = new QFormLayout;
    form->addRow("Type", typeCombo_);
    form->addRow("X", xSpin_);
    form->addRow("Y", ySpin_);
    form->addRow("Energy", energySpin_);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons_);

    connect(buttons_, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons_, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

CreatureFormData CreatureDialog::data() const
{
    return {
        typeCombo_->currentText(),
        xSpin_->value(),
        ySpin_->value(),
        energySpin_->value()
    };
}

void CreatureDialog::setData(const CreatureFormData& data)
{
    typeCombo_->setCurrentText(data.type);
    xSpin_->setValue(data.x);
    ySpin_->setValue(data.y);
    energySpin_->setValue(data.energy);
}
