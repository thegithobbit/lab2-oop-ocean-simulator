#pragma once

#include <QDialog>
#include <QString>

class QComboBox;
class QDialogButtonBox;
class QDoubleSpinBox;

struct CreatureFormData {
    QString type;
    double x = 0.0;
    double y = 0.0;
    double energy = 50.0;
};

class CreatureDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreatureDialog(QWidget *parent = nullptr);

    CreatureFormData data() const;
    void setData(const CreatureFormData& data);

private:
    QComboBox *typeCombo_;
    QDoubleSpinBox *xSpin_;
    QDoubleSpinBox *ySpin_;
    QDoubleSpinBox *energySpin_;
    QDialogButtonBox *buttons_;
};
