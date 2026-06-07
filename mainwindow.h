#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Ocean.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QCheckBox;
class QComboBox;
class QGraphicsScene;
class QGraphicsView;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QSlider;
class QSpinBox;
class QStackedWidget;
class QTableWidget;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *buildSimulationPage();
    QWidget *buildCreaturesPage();
    QWidget *buildSettingsPage();
    QWidget *buildStatisticsPage();

    void refreshAll();
    void refreshScene();
    void refreshTable();
    void refreshStats();
    void addLogMessage(const QString& message);
    QString buildReportText() const;
    int selectedCreatureId() const;

private slots:
    void showSimulationPage();
    void showCreaturesPage();
    void showSettingsPage();
    void showStatisticsPage();
    void startSimulation();
    void pauseSimulation();
    void stepSimulation();
    void resetSimulation();
    void addCreature();
    void editCreature();
    void deleteCreature();
    void applySettings();
    void exportReport();
    void clearLog();
    void onSearchChanged(const QString& text);
    void onTypeFilterChanged(const QString& type);
    void onSpeedChanged(int value);
    void onTableSelectionChanged();

private:
    Ui::MainWindow *ui;
    Ocean ocean_;
    QTimer *timer_;

    QStackedWidget *screens_;
    QGraphicsScene *scene_;
    QGraphicsView *oceanView_;
    QTableWidget *creaturesTable_;
    QListWidget *eventLog_;
    QLineEdit *searchEdit_;
    QComboBox *typeFilter_;
    QSlider *speedSlider_;
    QSpinBox *widthSpin_;
    QSpinBox *heightSpin_;
    QCheckBox *showGridCheck_;
    QLabel *statusLabel_;
    QLabel *totalLabel_;
    QLabel *aliveLabel_;
    QLabel *fishLabel_;
    QLabel *sharkLabel_;
    QLabel *planktonLabel_;
    QLabel *energyLabel_;
    QPushButton *editButton_;
    QPushButton *deleteButton_;
};
#endif // MAINWINDOW_H
