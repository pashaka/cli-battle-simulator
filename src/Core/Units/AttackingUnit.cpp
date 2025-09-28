//
// Created by Carpov Pavel on 28.09.2025.
//

#include "AttackingUnit.hpp"

#include "Core/Engine/MapUnitsController.hpp"
#include "Core/Engine/Util.hpp"
#include "IO/System/EventLog.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"

namespace sw::core
{
	bool MeleeAttackingUnit::tryToExecuteMeleeAttack(const MapUnitsController& worldState)
	{
		// find adjacent enemy units
		std::vector<std::shared_ptr<Unit>> adjacentEnemies =
			worldState.getUnitsInRange(this->getPosition(), MAX_MELEE_ATTACK_RANGE, MIN_MELEE_ATTACK_RANGE);
		std::erase_if(adjacentEnemies, [](const auto& u) { return !u->canTakeMeleeDamage(); });

		if (adjacentEnemies.empty())
		{
			return false;
		}

		// attack a random adjacent enemy
		uint32_t targetIndex = Util::randomInRange(static_cast<uint32_t>(adjacentEnemies.size() - 1));
		auto targetUnit = adjacentEnemies[targetIndex];

		auto damage = this->getStrength(); // uint32_t
		// apply damage
		targetUnit->increaseHp(-static_cast<int32_t>(damage));

		// obtain shared_ptr for this unit so we can log its id
		auto unitPtr = this->shared_from_this();
		// Log attack and possible death using the injected EventLog on worldState
		worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitAttacked{unitPtr->getId(), targetUnit->getId(), damage, targetUnit->getHp().value()});
		if (targetUnit->getHp().value() == 0)
		{
			worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitDied{targetUnit->getId()});
		}

		return true;
	}

	bool RangedAttackingUnit::tryToExecuteRangedAttack(const MapUnitsController& worldState) const
	{
		// Rule for HunterUnit and maybe other
		if (this->disallowRangedIfAdjacent())
		{
			auto adjacentUnits = worldState.getUnitsInRange(this->getPosition(), MAX_MELEE_ATTACK_RANGE, 1);
			// erase units that can take melee damage
			std::erase_if(adjacentUnits, [](const auto& u) { return !u->canTakeMeleeDamage(); });
			if (!adjacentUnits.empty()) // adjacent units present, disallow ranged attack
			{
				return false;
			}
		}

		std::vector<std::shared_ptr<Unit>> attackableUnits =
			worldState.getUnitsInRange(this->getPosition(), getRange(), MIN_RANGED_ATTACK_RANGE);
		std::erase_if(attackableUnits, [](const auto& u) { return !u->canTakeRangedDamage(); });

		if (attackableUnits.empty())
		{
			return false;
		}
		// pick random target
		auto targetUnit = attackableUnits[ Util::randomInRange(static_cast<uint32_t>(attackableUnits.size()) - 1, 0) ];
		// construct AttackAction using shared_ptr so Action will keep a weak_ptr

		auto damage = this->getAgility(); // uint32_t
		// apply damage
		targetUnit->increaseHp(-static_cast<int32_t>(damage));

		// obtain shared_ptr for this unit so we can log its id
		auto unitPtr = this->shared_from_this();
		// Log attack and possible death using the injected EventLog on worldState
		worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitAttacked{unitPtr->getId(), targetUnit->getId(), damage, targetUnit->getHp().value()});
		if (targetUnit->getHp().value() == 0)
		{
			worldState.eventLog_.log(worldState.getCurrentTick(), sw::io::UnitDied{targetUnit->getId()});
		}

		return true;
	}
}
