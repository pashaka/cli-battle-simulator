#include "Core/Engine/MapUnitsController.hpp"

#include <Core/Engine/Action.hpp>
#include <Core/Units/Unit.hpp>
#include <memory>

namespace sw::core
{
    bool Unit::tryToExecuteNextAction(const MapUnitsController& worldState)
    {
        for (auto & actionType: getActionTypesOrder())
        {
            if (actionType->tryToExecute(shared_from_this(), worldState))
			{
            	consumeAction();
            	onActionExecuted(actionType);
            	return true;
			}
        }
    	WaitActionType.tryToExecute(shared_from_this(), worldState);
    	onActionExecuted(&WaitActionType);
        return false;
    }
}
