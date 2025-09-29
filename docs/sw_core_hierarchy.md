# sw::core — Class hierarchy

This document describes the main classes in the `sw::core` namespace and their relationships. It is a Markdown-friendly presentation of the previously generated DOT diagram.

## Overview (ASCII UML)

```
Unit
├─ MovingUnit (virtual Unit)
├─ MeleeAttackingUnit (virtual Unit)
├─ RangedAttackingUnit (virtual Unit)
├─ TriggeredUnit (virtual Unit)
├─ ExplodingUnit (virtual Unit)
└─ HealingUnit (virtual Unit)

Concrete units (mixins combined):
- SwordsmanUnit : MovingUnit + MeleeAttackingUnit
- HunterUnit   : MovingUnit + MeleeAttackingUnit + RangedAttackingUnit
- TowerUnit    : Unit
- HealerUnit   : MovingUnit + HealingUnit
- MineUnit     : ExplodingUnit + TriggeredUnit

MapUnitsController (owns units, runs turns, logs events)
ActionTypeBase (action singletons used for dispatch)
```

## Class summary

- `Unit` (base)
  - Core state: id, optional hp, name, position, available actions, solid flag
  - Important methods: getId(), getHp(), increaseHp(), setHp(), isAlive(), getActionTypesOrder() (pure virtual), consumeAction()

- Mixin/behaviour classes (usually `public virtual Unit`):
  - `MovingUnit` — movement target, speed, tryToExecuteMove(worldState)
  - `MeleeAttackingUnit` — strength, tryToExecuteMeleeAttack(worldState)
  - `RangedAttackingUnit` — agility/power and range, tryToExecuteRangedAttack(worldState), disallowRangedIfAdjacent()
  - `TriggeredUnit` — trigger state and range, tryToExecuteTrigger(worldState)
  - `ExplodingUnit` — explosion power/range, tryToExecuteExplode(worldState), shouldExplodeNow()
  - `HealingUnit` — spirit and heal range, tryToExecuteHeal(worldState)

- Concrete unit types (compose behaviours):
  - `SwordsmanUnit` = MovingUnit + MeleeAttackingUnit
  - `HunterUnit` = MovingUnit + MeleeAttackingUnit + RangedAttackingUnit
  - `TowerUnit` = Unit (stationary ranged)
  - `HealerUnit` = MovingUnit + HealingUnit
  - `MineUnit` = ExplodingUnit + TriggeredUnit (non-moving)

- Engine/controller and helpers
  - `MapUnitsController` — map size, unit storage, turn loop, event logging, getUnitsInRange(), getCurrentTick()
  - `ActionTypeBase` — action dispatch singletons (MoveActionType, MeleeAttackActionType, RangedAttackActionType, ExplodeAttackActionType, HealActionType, TriggerActionType, ...)

## Event logging
Units use the injected `EventLog` (available through `MapUnitsController::eventLog_`) to emit events such as `UnitAttacked`, `UnitDied`, `UnitMoved`, etc. `MapUnitsController` provides `getCurrentTick()` to obtain the simulation tick for event timestamps.

## Notes & recommendations
- Behaviour classes are declared `virtual` in inheritance so multiple mixins can be combined without duplicate `Unit` bases.
- Concrete unit types provide `getActionTypesOrder()` which returns an ordered list of `ActionTypeBase*` singletons. The engine iterates that list to ask each action type to try execution for the unit.
- If you want a graphical image, render `docs/sw_core_hierarchy.dot` with Graphviz (e.g. `dot -Tpng docs/sw_core_hierarchy.dot -o docs/sw_core_hierarchy.png`).

If you want this README content copied into the repository root README or exported as an SVG/PNG, I can do that next.
