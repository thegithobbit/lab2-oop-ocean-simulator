#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "creaturedialog.h"

#include "core/entities/fish.h"
#include "core/entities/plankton.h"
#include "core/entities/shark.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QAbstractItemView>
#include <QBrush>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      timer_(new QTimer(this)),
      screens_(new QStackedWidget(this)),
      scene_(new QGraphicsScene(this)),
      oceanView_(nullptr),
      creaturesTable_(nullptr),
      searchEdit_(nullptr),
      typeFilter_(nullptr),
      speedSlider_(nullptr),
      widthSpin_(nullptr),
      heightSpin_(nullptr),
      showGridCheck_(nullptr),
      statusLabel_(new QLabel("Ready", this)),
      totalLabel_(nullptr),
      aliveLabel_(nullptr),
      fishLabel_(nullptr),
      sharkLabel_(nullptr),
      planktonLabel_(nullptr),
      energyLabel_(nullptr),
      editButton_(nullptr),
      deleteButton_(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Ocean Simulator");
    resize(1000, 700);

    auto *toolbar = addToolBar("Navigation");
    toolbar->addAction("Simulation", this, &MainWindow::showSimulationPage);
    toolbar->addAction("Creatures", this, &MainWindow::showCreaturesPage);
    toolbar->addAction("Settings", this, &MainWindow::showSettingsPage);
    toolbar->addAction("Statistics", this, &MainWindow::showStatisticsPage);

    screens_->addWidget(buildSimulationPage());
    screens_->addWidget(buildCreaturesPage());
    screens_->addWidget(buildSettingsPage());
    screens_->addWidget(buildStatisticsPage());
    setCentralWidget(screens_);
    statusBar()->addWidget(statusLabel_);

    connect(timer_, &QTimer::timeout, this, &MainWindow::stepSimulation);
    connect(&ocean_, &Ocean::changed, this, &MainWindow::refreshAll);

    refreshAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QWidget *MainWindow::buildSimulationPage()
{
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    auto *buttons = new QHBoxLayout;

    auto *startButton = new QPushButton("Start", page);
    auto *pauseButton = new QPushButton("Pause", page);
    auto *stepButton = new QPushButton("Step", page);
    auto *resetButton = new QPushButton("Reset", page);
    auto *addButton = new QPushButton("Add creature", page);

    buttons->addWidget(startButton);
    buttons->addWidget(pauseButton);
    buttons->addWidget(stepButton);
    buttons->addWidget(resetButton);
    buttons->addWidget(addButton);
    buttons->addStretch();

    oceanView_ = new QGraphicsView(scene_, page);
    oceanView_->setMinimumHeight(420);
    oceanView_->setRenderHint(QPainter::Antialiasing);

    layout->addLayout(buttons);
    layout->addWidget(oceanView_);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseSimulation);
    connect(stepButton, &QPushButton::clicked, this, &MainWindow::stepSimulation);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetSimulation);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addCreature);

    return page;
}

QWidget *MainWindow::buildCreaturesPage()
{
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    auto *filters = new QHBoxLayout;

    searchEdit_ = new QLineEdit(page);
    searchEdit_->setPlaceholderText("Search by type or id");
    typeFilter_ = new QComboBox(page);
    typeFilter_->addItems({"All", "Fish", "Shark", "Plankton"});

    auto *addButton = new QPushButton("Add", page);
    editButton_ = new QPushButton("Edit", page);
    deleteButton_ = new QPushButton("Delete", page);
    editButton_->setEnabled(false);
    deleteButton_->setEnabled(false);

    filters->addWidget(new QLabel("Search", page));
    filters->addWidget(searchEdit_);
    filters->addWidget(new QLabel("Type", page));
    filters->addWidget(typeFilter_);
    filters->addWidget(addButton);
    filters->addWidget(editButton_);
    filters->addWidget(deleteButton_);

    creaturesTable_ = new QTableWidget(page);
    creaturesTable_->setColumnCount(6);
    creaturesTable_->setHorizontalHeaderLabels({"ID", "Type", "X", "Y", "Energy", "Alive"});
    creaturesTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    creaturesTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    creaturesTable_->setSelectionMode(QAbstractItemView::SingleSelection);
    creaturesTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addLayout(filters);
    layout->addWidget(creaturesTable_);

    connect(searchEdit_, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(typeFilter_, &QComboBox::currentTextChanged, this, &MainWindow::onTypeFilterChanged);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addCreature);
    connect(editButton_, &QPushButton::clicked, this, &MainWindow::editCreature);
    connect(deleteButton_, &QPushButton::clicked, this, &MainWindow::deleteCreature);
    connect(creaturesTable_, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);

    return page;
}

QWidget *MainWindow::buildSettingsPage()
{
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    auto *form = new QFormLayout;

    speedSlider_ = new QSlider(Qt::Horizontal, page);
    speedSlider_->setRange(1, 10);
    speedSlider_->setValue(5);

    widthSpin_ = new QSpinBox(page);
    widthSpin_->setRange(10, 500);
    widthSpin_->setValue(ocean_.width());

    heightSpin_ = new QSpinBox(page);
    heightSpin_->setRange(10, 500);
    heightSpin_->setValue(ocean_.height());

    showGridCheck_ = new QCheckBox("Show grid", page);
    showGridCheck_->setChecked(true);

    auto *applyButton = new QPushButton("Apply settings", page);
    auto *statsButton = new QPushButton("Open statistics", page);

    form->addRow("Simulation speed", speedSlider_);
    form->addRow("Ocean width", widthSpin_);
    form->addRow("Ocean height", heightSpin_);
    form->addRow("Grid", showGridCheck_);

    layout->addLayout(form);
    layout->addWidget(applyButton);
    layout->addWidget(statsButton);
    layout->addStretch();

    connect(speedSlider_, &QSlider::valueChanged, this, &MainWindow::onSpeedChanged);
    connect(showGridCheck_, &QCheckBox::toggled, this, &MainWindow::refreshScene);
    connect(applyButton, &QPushButton::clicked, this, &MainWindow::applySettings);
    connect(statsButton, &QPushButton::clicked, this, &MainWindow::showStatisticsPage);

    return page;
}

QWidget *MainWindow::buildStatisticsPage()
{
    auto *page = new QWidget(this);
    auto *layout = new QGridLayout(page);

    totalLabel_ = new QLabel(page);
    aliveLabel_ = new QLabel(page);
    fishLabel_ = new QLabel(page);
    sharkLabel_ = new QLabel(page);
    planktonLabel_ = new QLabel(page);
    energyLabel_ = new QLabel(page);

    auto *resetButton = new QPushButton("Reset demo data", page);
    auto *creaturesButton = new QPushButton("Open creature table", page);

    layout->addWidget(new QLabel("Total creatures", page), 0, 0);
    layout->addWidget(totalLabel_, 0, 1);
    layout->addWidget(new QLabel("Alive creatures", page), 1, 0);
    layout->addWidget(aliveLabel_, 1, 1);
    layout->addWidget(new QLabel("Fish", page), 2, 0);
    layout->addWidget(fishLabel_, 2, 1);
    layout->addWidget(new QLabel("Sharks", page), 3, 0);
    layout->addWidget(sharkLabel_, 3, 1);
    layout->addWidget(new QLabel("Plankton", page), 4, 0);
    layout->addWidget(planktonLabel_, 4, 1);
    layout->addWidget(new QLabel("Average energy", page), 5, 0);
    layout->addWidget(energyLabel_, 5, 1);
    layout->addWidget(resetButton, 6, 0);
    layout->addWidget(creaturesButton, 6, 1);
    layout->setRowStretch(7, 1);

    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetSimulation);
    connect(creaturesButton, &QPushButton::clicked, this, &MainWindow::showCreaturesPage);

    return page;
}

void MainWindow::refreshAll()
{
    refreshScene();
    refreshTable();
    refreshStats();
}

void MainWindow::refreshScene()
{
    scene_->clear();
    scene_->setSceneRect(0, 0, ocean_.width() * 6, ocean_.height() * 6);

    if (showGridCheck_ && showGridCheck_->isChecked()) {
        for (int x = 0; x <= ocean_.width(); x += 10) {
            scene_->addLine(x * 6, 0, x * 6, ocean_.height() * 6, QPen(QColor(220, 230, 235)));
        }
        for (int y = 0; y <= ocean_.height(); y += 10) {
            scene_->addLine(0, y * 6, ocean_.width() * 6, y * 6, QPen(QColor(220, 230, 235)));
        }
    }

    for (const auto& creature : ocean_.getCreatures()) {
        QColor color = QColor("#2f80ed");
        if (creature->getType() == "Shark") {
            color = QColor("#d64545");
        } else if (creature->getType() == "Plankton") {
            color = QColor("#27ae60");
        }

        auto *item = scene_->addEllipse(creature->getX() * 6, creature->getY() * 6, 14, 14, QPen(Qt::black), QBrush(color));
        item->setToolTip(QString("%1 #%2, energy %3")
                             .arg(QString::fromStdString(creature->getType()))
                             .arg(creature->getId())
                             .arg(creature->getEnergy(), 0, 'f', 1));
    }
}

void MainWindow::refreshTable()
{
    const QString search = searchEdit_ ? searchEdit_->text().trimmed().toLower() : QString();
    const QString typeFilter = typeFilter_ ? typeFilter_->currentText() : "All";

    creaturesTable_->setRowCount(0);
    for (const auto& creature : ocean_.getCreatures()) {
        const QString id = QString::number(creature->getId());
        const QString type = QString::fromStdString(creature->getType());
        if (typeFilter != "All" && type != typeFilter) {
            continue;
        }
        if (!search.isEmpty() && !id.contains(search) && !type.toLower().contains(search)) {
            continue;
        }

        const int row = creaturesTable_->rowCount();
        creaturesTable_->insertRow(row);
        creaturesTable_->setItem(row, 0, new QTableWidgetItem(id));
        creaturesTable_->setItem(row, 1, new QTableWidgetItem(type));
        creaturesTable_->setItem(row, 2, new QTableWidgetItem(QString::number(creature->getX(), 'f', 1)));
        creaturesTable_->setItem(row, 3, new QTableWidgetItem(QString::number(creature->getY(), 'f', 1)));
        creaturesTable_->setItem(row, 4, new QTableWidgetItem(QString::number(creature->getEnergy(), 'f', 1)));
        creaturesTable_->setItem(row, 5, new QTableWidgetItem(creature->isAlive() ? "Yes" : "No"));
    }
    onTableSelectionChanged();
}

void MainWindow::refreshStats()
{
    const auto stats = ocean_.stats();
    totalLabel_->setText(QString::number(stats.total));
    aliveLabel_->setText(QString::number(stats.alive));
    fishLabel_->setText(QString::number(stats.fish));
    sharkLabel_->setText(QString::number(stats.sharks));
    planktonLabel_->setText(QString::number(stats.plankton));
    energyLabel_->setText(QString::number(stats.averageEnergy, 'f', 1));
}

int MainWindow::selectedCreatureId() const
{
    const auto selected = creaturesTable_->selectedItems();
    if (selected.isEmpty()) {
        return -1;
    }
    return creaturesTable_->item(selected.first()->row(), 0)->text().toInt();
}

void MainWindow::showSimulationPage()
{
    screens_->setCurrentIndex(0);
}

void MainWindow::showCreaturesPage()
{
    screens_->setCurrentIndex(1);
}

void MainWindow::showSettingsPage()
{
    screens_->setCurrentIndex(2);
}

void MainWindow::showStatisticsPage()
{
    screens_->setCurrentIndex(3);
}

void MainWindow::startSimulation()
{
    timer_->start(1100 - speedSlider_->value() * 100);
    statusLabel_->setText("Simulation is running");
}

void MainWindow::pauseSimulation()
{
    timer_->stop();
    statusLabel_->setText("Simulation paused");
}

void MainWindow::stepSimulation()
{
    ocean_.step();
    statusLabel_->setText("One simulation step completed");
}

void MainWindow::resetSimulation()
{
    timer_->stop();
    ocean_.seedDemoData();
    statusLabel_->setText("Demo data restored");
}

void MainWindow::addCreature()
{
    CreatureDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    const auto data = dialog.data();
    if (data.type == "Shark") {
        ocean_.addCreature(std::make_shared<Shark>(data.x, data.y, data.energy));
    } else if (data.type == "Plankton") {
        ocean_.addCreature(std::make_shared<Plankton>(data.x, data.y, data.energy));
    } else {
        ocean_.addCreature(std::make_shared<Fish>(data.x, data.y, data.energy));
    }
    statusLabel_->setText("Creature added");
}

void MainWindow::editCreature()
{
    const int id = selectedCreatureId();
    if (id < 0) {
        return;
    }

    for (const auto& creature : ocean_.getCreatures()) {
        if (creature->getId() != id) {
            continue;
        }

        CreatureDialog dialog(this);
        dialog.setData({
            QString::fromStdString(creature->getType()),
            creature->getX(),
            creature->getY(),
            creature->getEnergy()
        });

        if (dialog.exec() == QDialog::Accepted) {
            const auto data = dialog.data();
            ocean_.updateCreature(id, data.type, data.x, data.y, data.energy);
            statusLabel_->setText("Creature updated");
        }
        return;
    }
}

void MainWindow::deleteCreature()
{
    const int id = selectedCreatureId();
    if (id < 0) {
        return;
    }

    if (QMessageBox::question(this, "Delete creature", "Delete selected creature?") == QMessageBox::Yes) {
        ocean_.removeCreature(id);
        statusLabel_->setText("Creature deleted");
    }
}

void MainWindow::applySettings()
{
    ocean_.setSize(widthSpin_->value(), heightSpin_->value());
    statusLabel_->setText("Settings applied");
}

void MainWindow::onSearchChanged(const QString&)
{
    refreshTable();
}

void MainWindow::onTypeFilterChanged(const QString&)
{
    refreshTable();
}

void MainWindow::onSpeedChanged(int value)
{
    if (timer_->isActive()) {
        timer_->start(1100 - value * 100);
    }
    statusLabel_->setText(QString("Speed: %1").arg(value));
}

void MainWindow::onTableSelectionChanged()
{
    const bool hasSelection = selectedCreatureId() >= 0;
    editButton_->setEnabled(hasSelection);
    deleteButton_->setEnabled(hasSelection);
}
