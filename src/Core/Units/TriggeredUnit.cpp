//
// Created by Carpov Pavel on 28.09.2025.
//

#include "TriggeredUnit.hpp"
#include "Core/Engine/MapUnitsController.hpp"

namespace sw::core
{
	bool TriggeredUnit::tryToExecuteTrigger(const MapUnitsController& worldState)
	{
		auto nearbyUnits = worldState.getUnitsInRange(this->getPosition(), triggerRange);
		// remove non-solid units
		std::erase_if(nearbyUnits, [](const auto& u) { return !u->isSolid(); });
		if (!nearbyUnits.empty())
		{
			triggered = true;
			return true;
		}
		return false;
	}
}

