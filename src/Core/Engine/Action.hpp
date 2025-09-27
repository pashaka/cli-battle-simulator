//
// Created by Carpov Pavel on 26.09.2025.
//

#ifndef SW_BATTLE_TEST_ACTION_HPP
#define SW_BATTLE_TEST_ACTION_HPP
#include "Core/Units/Unit.hpp"

#include <memory>

namespace sw::core
{
	// Base class for actions that units can perform
	class Action
	{
	public:
		ActionType actionType{LastActionType};
		std::weak_ptr <Unit> unitFrom; // non-owning reference to unit performing the action
		std::vector< std::weak_ptr <Unit> > unitsTo; // non-owning references to target units (if applicable)
		Coordinate coordinateFrom; // source coordinate (if applicable)
		std::optional<Coordinate> coordinateTo; // target coordinate (if applicable)
	};

	class MoveAction : public Action
	{
	public:
		MoveAction(std::shared_ptr<Unit> unit, Coordinate coordinateTo)
			: Action()
		{
			unitFrom = unit;
			this->coordinateFrom = unit->getPosition();
			this->coordinateTo = coordinateTo;
			this->actionType = MoveActionType;
		}
	};

	class MeleeAttackAction : public Action
	{
	public:
		MeleeAttackAction(std::shared_ptr<Unit> unit, std::shared_ptr<Unit> targetUnit)
			: Action()
		{
			unitFrom = unit;
			this->coordinateFrom = unit->getPosition();
			this->coordinateTo = targetUnit->getPosition();
			this->unitsTo.emplace_back(targetUnit);
			this->actionType = MeleeAttackActionType;
		}
	};

	class RangedAttackAction : public Action
	{
	public:
		RangedAttackAction(std::shared_ptr<Unit> unit, std::shared_ptr<Unit> targetUnit)
		{
			unitFrom = unit;
			this->coordinateFrom = unit->getPosition();
			this->coordinateTo = targetUnit->getPosition();
			this->unitsTo.emplace_back(targetUnit);
			this->actionType = RangedAttackActionType;
		}
	};

	class WaitAction : public Action
	{
	public:
		WaitAction(std::shared_ptr<Unit> unit)
		{
			unitFrom = unit;
			this->coordinateFrom = unit->getPosition();
			this->actionType = WaitActionType;
		}
	};
}
#endif	//SW_BATTLE_TEST_ACTION_HPP
