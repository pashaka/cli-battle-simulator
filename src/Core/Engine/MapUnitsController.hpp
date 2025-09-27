//
// Created by Carpov Pavel on 25.09.2025.
//

#ifndef SW_BATTLE_TEST_BATTLEMAP_HPP
#define SW_BATTLE_TEST_BATTLEMAP_HPP
#include "Action.hpp"
#include "Coordinate.hpp"
#include "Core/Units/Unit.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <vector>

namespace sw { class EventLog; }

namespace sw::core
{
	// Holds map dimensions and units on the map
	class MapUnitsController
	{
		// No matrix or grid for now. Just dimensions and a flat collection of units.
		uint32_t width{};
		uint32_t height{};
		std::map<uint32_t, std::shared_ptr<Unit> > units; // store units by id (controller owns units)
		sw::EventLog& eventLog_; // non-owning reference, injected
		// callback to obtain current tick/round from owner (Engine)
		std::function<uint64_t()> getCurrentTick_;

	public:
		MapUnitsController(uint32_t w, uint32_t h, sw::EventLog& eventLog, std::function<uint64_t()> getCurrentTick) :
			width(w), height(h), eventLog_(eventLog), getCurrentTick_(std::move(getCurrentTick))
		{
			if (width == 0 || height == 0)
			{
				throw std::runtime_error("invalid map dimensions");
			}
		}

		[[nodiscard]] uint32_t getWidth() const noexcept { return width; }
		[[nodiscard]] uint32_t getHeight() const noexcept { return height; }

		bool isValidCoordinate(const Coordinate& c) const { return c.getX() >= 0 && c.getY() >= 0 && static_cast<uint32_t>(c.getX()) < width && static_cast<uint32_t>(c.getY()) < height; }
		bool isOccupied(const Coordinate& c) const;

		// take ownership of the provided unit and place it on the map (SPAWN)
		void placeUnit(std::unique_ptr<Unit> unit);

		// returns number of actions performed in this turn
		uint32_t doTurn();
		void handleNextRound();

		bool assignMarchCommand(uint32_t unitId, int32_t targetX, int32_t targetY);

		std::optional<Action> decideActionForUnit(const std::shared_ptr<Unit>& unit, const ActionType& actionType);
		const void executeAction(const Action& action);

		const std::optional<Action> decideMoveActionForUnit(const std::shared_ptr<MovingUnit>& unit);
		const void executeMoveAction(const Action& action);

		// decide attack action
		const std::optional<Action> decideAttackActionForUnit(const ActionType& actionType, const std::shared_ptr<Unit>& unit);
		const void executeAttackAction(const Action& action);

		// returns units that are within the given range from the position (range_min <= unit <= range_max)
		std::vector<Coordinate> getCoordinatesInRange(const Coordinate& position, uint32_t range_max, uint32_t range_min = 1) const;
		[[nodiscard]] std::vector<std::shared_ptr<Unit>> getUnitsInRange(const Coordinate& position, uint32_t range_max, uint32_t range_min = 1) const;

		uint32_t removeDeadUnits();
		uint32_t getUnitsCount() const { return units.size(); }
		void printMap();
	};
}

#endif	//SW_BATTLE_TEST_BATTLEMAP_HPP
