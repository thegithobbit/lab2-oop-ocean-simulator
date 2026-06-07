# Ocean Simulator: GUI elements summary

## Screens and forms

1. Simulation screen
2. Creatures screen
3. Settings screen
4. Statistics screen
5. Creature dialog for adding and editing creatures

Total: 5 screens/forms.

## Control elements

- Navigation toolbar actions: 4
- Simulation buttons: Start, Pause, Step, Reset, Add creature, Statistics: 6
- Creatures screen controls: search field, type filter, Add, Edit, Delete: 5
- Creatures table: 1
- Settings controls: speed slider, width spin box, height spin box, grid checkbox, Apply settings, Open statistics: 6
- Statistics controls: Reset demo data, Open creature table, Export report, Clear log: 4
- Statistics labels: Total, Alive, Fish, Sharks, Plankton, Average energy: 6
- Event log list: 1
- Creature dialog controls: type combo box, X spin box, Y spin box, energy spin box, OK, Cancel: 6

Total: 39 control elements.

## Container controls

- QGraphicsView for ocean visualization
- QTableWidget for creatures
- QListWidget for event log
- QStackedWidget for screens

Total: 4 container controls.

## Event handlers

1. Open Simulation screen
2. Open Creatures screen
3. Open Settings screen
4. Open Statistics screen
5. Start simulation
6. Pause simulation
7. Run one simulation step
8. Reset demo data
9. Add creature
10. Edit selected creature
11. Delete selected creature
12. Apply settings
13. Export report
14. Clear event log
15. Filter table by search text
16. Filter table by creature type
17. Change simulation speed
18. React to table row selection
19. Toggle grid visibility
20. Timer tick for automatic simulation steps

Total: 20 event handlers.

## Responsibility separation

- Simulation logic: Ocean, ICreature, Creature, Fish, Shark, Plankton.
- GUI logic: MainWindow, CreatureDialog.
- Data visualization: QGraphicsScene/QGraphicsView in MainWindow.
- Data management and statistics: Ocean and OceanStats.
