//
// Created by Carpov Pavel on 29.09.2025.
//

#include "Core/Engine/Action.hpp"
#include "Core/Units/Unit.hpp"

namespace sw::core
{
	bool WaitActionTypeClass::tryToExecute(
		const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState)
	{
		unit->consumeAction();
		// false means no action performed
		return false;
	}
}