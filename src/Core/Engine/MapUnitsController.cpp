//
// Created by Carpov Pavel on 25.09.2025.
//

#include "MapUnitsController.hpp"

#include "Action.hpp"
#include "Core/Units/MovingUnit.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/System/EventLog.hpp"
#include "Util.hpp"

#include <cassert>
#include <random>
#include <unordered_set>

namespace sw::core
{

	bool MapUnitsController::isOccupied(const Coordinate& c) const
	{
		for (const auto& unit : units)
		{
			if (unit.second->getPosition() == c && unit.second->isSolid())
			{
				return true;
			}
		}
		return false;
	}

	// Places unit on the map. Steals ownership
	void MapUnitsController::placeUnit(std::unique_ptr<Unit> unit)
	{
		assert(unit && "MapUnitsController::placeUnit: null unit");
		// check position validity
		const Coordinate& pos = unit->getPosition();
		assert(isValidCoordinate(pos) && "MapUnitsController::placeUnit: invalid unit position");
		uint32_t id = unit->getId();
		if (units.find(id) != units.end())
		{
			throw std::runtime_error("BattleMap::placeUnit: unit with same id already exists");
		}
		// store ownership: convert unique_ptr -> shared_ptr
		units[id] = std::move(unit);
	}

	void MapUnitsController::handleNextRound()
	{
		// reset available actions for all units
		for (auto& [id, unit] : units)
		{
			unit->setAvailableActionsPerTurn(unit->getDefaultUnitActionsPerTurn());
		}
	}

	bool MapUnitsController::assignMarchCommand(uint32_t unitId, int32_t targetX, int32_t targetY)
	{
		// find unit by id
		auto it = units.find(unitId);
		if (it == units.end())
		{
			throw std::runtime_error("BattleMap: No unit found");
		}
		auto movingUnit = std::dynamic_pointer_cast<MovingUnit>(it->second);
		if (!movingUnit)
		{
			throw std::runtime_error("BattleMap: No moving unit found");
		}
		movingUnit->setTarget(Coordinate(static_cast<int32_t>(targetX), static_cast<int32_t>(targetY)));
		return true;
	}

	std::vector<Coordinate> MapUnitsController::getCoordinatesInRange(const Coordinate& position, uint32_t range_max, uint32_t range_min) const
	{
		std::vector<Coordinate> coordinates;
		coordinates.reserve((range_max * 2 + 1) * (range_max * 2 + 1)); // reserve enough to avoid reallocations
		for (int32_t dx = -static_cast<int32_t>(range_max); dx <= static_cast<int32_t>(range_max); dx++)
		{
			for (int32_t dy = -static_cast<int32_t>(range_max); dy <= static_cast<int32_t>(range_max); dy++)
			{
				Coordinate coord(position + Coordinate(dx, dy));
				if (!isValidCoordinate(coord))
				{
					continue;
				}
				// range_max check may be redundant, but let's be safe
				if (position.distance(coord) >= range_min && position.distance(coord) <= range_max)
				{
					coordinates.push_back(coord);
				}
			}
		}
		return coordinates;
	}

	std::vector< std::shared_ptr<Unit> > MapUnitsController::getUnitsInRange(const Coordinate& position, const uint32_t range_max, const uint32_t range_min) const
	{
		// loop thru units
		std::vector< std::shared_ptr<Unit> > result;
		result.reserve(units.size()); // reserve enough to avoid reallocations
		for (const auto& [id, unit] : units)
		{
			uint32_t distance = position.distance(unit->getPosition());
			if (distance <= range_max && distance >= range_min)
			{
				result.emplace_back(unit);
			}
		}
		return result;
	}

	uint32_t MapUnitsController::removeDeadUnits()
	{
		return std::erase_if(units, [](const auto& pair) { return !pair.second->isAlive(); });
	}

	void MapUnitsController::printMap()
	{// print the map with 3 characters per cell
		for (uint32_t y = 0; y < height; ++y)
		{
			for (uint32_t x = 0; x < width; ++x)
			{
				Coordinate c(x, y);
				auto units = getUnitsInRange(c, 0,0);
				// print first 2 letters of unit name or [  ] if empty
				auto unitsHere = getUnitsInRange(c, 0,0);
				std::cout << (units.size() ? "["+units.back()->getShortName()+"]" : "[  ]");
			}
			std::cout << std::endl;
		}
	}

	uint32_t MapUnitsController::doTurn()
	{
		uint32_t result{};
		// loop thru units and let them act
		for (auto& [id, unit] : units)
		{
			// check for units that were killed during this round and not yet removed
			if (!unit->isAlive())
			{
				continue; // skip dead units
			}
			while (unit->getAvailableActionsPerTurn() > 0)
			{
				if (unit->tryToExecuteNextAction(*this))
				{
					result++;
				}
			}
		}
		return result;
	}
	//
	// const void MapUnitsController::executeMoveAction(const Action& action)
	// {
	// 	auto unitPtr = action.unitFrom.lock();
	// 	if (!unitPtr)
	// 	{
	// 		throw std::runtime_error("MoveAction is missing unitFrom");
	// 	}
	// 	// cast to MovingUnit
	// 	auto movingUnitPtr = std::dynamic_pointer_cast<MovingUnit>(unitPtr);
	// 	if (!movingUnitPtr)
	// 	{
	// 		throw std::runtime_error("MoveAction can not be applied to non MovingUnit");
	// 	}
	// 	if (!action.coordinateTo.has_value())
	// 	{
	// 		throw std::runtime_error("MoveAction missing coordinateTo");
	// 	}
	// 	// perform move
	// 	auto newPos = action.coordinateTo.value();
	// 	unitPtr->setPosition(newPos);
	// 	// emit UnitMoved event using injected EventLog and current tick callback
	// 	eventLog_.log(getCurrentTick_(), sw::io::UnitMoved{unitPtr->getId(), static_cast<uint32_t>(newPos.getX()), static_cast<uint32_t>(newPos.getY())});
	// 	if (movingUnitPtr->getTarget() == newPos)
	// 	{
	// 		eventLog_.log(getCurrentTick_(), sw::io::MarchEnded(unitPtr->getId(), static_cast<uint32_t>(newPos.getX()), static_cast<uint32_t>(newPos.getY())));
	// 		// reached target
	// 		movingUnitPtr->clearTarget();
	// 	}
	// }
	//
	// const void MapUnitsController::executeAttackAction(const Action& action)
	// {
	// 	auto unitPtr = action.unitFrom.lock();
	// 	if (!unitPtr)
	// 	{
	// 		throw std::runtime_error("MeleeAttackAction is missing unitFrom");
	// 	}
	// 	if (action.unitsTo.empty())
	// 	{
	// 		throw std::runtime_error("MeleeAttackAction is missing unitsTo");
	// 	}
	// 	auto targetUnitPtr = action.unitsTo[0].lock();
	// 	if (!targetUnitPtr)
	// 	{
	// 		throw std::runtime_error("MeleeAttackAction target unit is expired");
	// 	}
	// 	if (!targetUnitPtr->hasHp())
	// 	{
	// 		throw std::runtime_error("MeleeAttackAction target unit can not take damage or is dead");
	// 	}
	// 	// perform attack
	// 	uint32_t damage = unitPtr->getDamageForActionType(action.actionType);
	// 	targetUnitPtr->setHp(targetUnitPtr->getHp().value() - damage);
	//
	// 	eventLog_.log(getCurrentTick_(), sw::io::UnitAttacked{unitPtr->getId(), targetUnitPtr->getId(), damage, targetUnitPtr->getHp().value()});
	// 	if (targetUnitPtr->getHp() == 0)
	// 	{
	// 		eventLog_.log(getCurrentTick_(), sw::io::UnitDied{targetUnitPtr->getId()});
	// 	}
	// }
}
