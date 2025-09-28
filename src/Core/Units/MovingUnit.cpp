//
// Created by Carpov Pavel on 28.09.2025.
//

#include "MovingUnit.hpp"

#include "Core/Engine/MapUnitsController.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/System/EventLog.hpp"

namespace sw::core
{
	bool MovingUnit::tryToExecuteMove(const MapUnitsController& worldState)
	{
		if (!this->getTarget().has_value())
		{
			return false;
		}
		if (this->getTarget().value() == this->getPosition())
		{
			return false;
		}
		Coordinate targetCoord = this->getTarget().value();
		std::vector<Coordinate> moveRange = worldState.getCoordinatesInRange(this->getPosition(), this->getSpeed(), MIN_MOVE_RANGE);
		std::vector<std::pair<Coordinate, float>> moveOptions{};
		moveOptions.reserve(moveRange.size());
		for (Coordinate& coord : moveRange)
		{
			if (this->isSolid() && worldState.isOccupied(coord))
			{
				continue;
			}
			if (coord.isCloserThanThat(this->getPosition(), targetCoord))
			{
				moveOptions.emplace_back(coord, targetCoord.euclideanDistance(coord));
			}
		}
		if (moveOptions.empty())
		{
			return false;
		}
		std::sort(moveOptions.begin(), moveOptions.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
		Coordinate nextCoord = moveOptions[0].first;
		setPosition(nextCoord);

		// Log attack and possible death using the injected EventLog on worldState
		worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitMoved{this->getId(), static_cast<uint32_t>(nextCoord.getX()), static_cast<uint32_t>(nextCoord.getY())});
		return true;
	}
}