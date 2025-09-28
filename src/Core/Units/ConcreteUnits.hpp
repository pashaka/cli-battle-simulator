#pragma once

#include "AttackingUnit.hpp"
#include "ExplodingUnit.hpp"
#include "HealingUnit.hpp"
#include "MovingUnit.hpp"
#include "TriggeredUnit.hpp"
#include "Unit.hpp"

namespace sw::core
{
	class SwordsmanUnit : public MovingUnit, public MeleeAttackingUnit
	{
	public:
		explicit SwordsmanUnit(uint32_t strength_) : Unit(true), MeleeAttackingUnit(strength_) {}
		[[nodiscard]] const std::vector<ActionTypeBase*>& getActionTypesOrder() const noexcept
		{
			static const std::vector<ActionTypeBase*> types = {&MeleeAttackActionType, &MoveActionType};
			return types;
		}
	};

	inline constexpr uint32_t HUNTER_MIN_ATTACK_RANGE{2};
	class HunterUnit : public MovingUnit, public MeleeAttackingUnit, public RangedAttackingUnit
	{
	public:
		explicit HunterUnit(uint32_t strength_, uint32_t agility_, uint32_t range_max_)
			: Unit(true), MeleeAttackingUnit(strength_), RangedAttackingUnit(agility_, HUNTER_MIN_ATTACK_RANGE, range_max_) {}
		[[nodiscard]] const std::vector<ActionTypeBase*>& getActionTypesOrder() const noexcept
		{
			static const std::vector<ActionTypeBase*> types = {&RangedAttackActionType, &MeleeAttackActionType, &MoveActionType};
			return types;
		}

		// Hunters restrict ranged attacks when adjacent (melee) units are present.
		[[nodiscard]] bool disallowRangedIfAdjacent() const noexcept { return true; }
	};

	class TowerUnit : public Unit
	{
	public:
		TowerUnit() : Unit(true) {}
		[[nodiscard]] const std::vector<ActionTypeBase*>& getActionTypesOrder() const noexcept
		{
			static const std::vector<ActionTypeBase*> types = {&RangedAttackActionType};
			return types;
		}
	};

	class HealerUnit : public MovingUnit, public HealingUnit
	{
	public:
		HealerUnit(uint32_t spirit_, uint32_t healingRange_) : Unit(true), HealingUnit(spirit_, healingRange_) {}
		[[nodiscard]] const std::vector<ActionTypeBase*>& getActionTypesOrder() const noexcept
		{
			static const std::vector<ActionTypeBase*> types = {&HealActionType, &MoveActionType};
			return types;
		}
	};

	class MineUnit : public ExplodingUnit, public TriggeredUnit// not MovingUnit
	{
	public:
		MineUnit(uint32_t power_, uint32_t trigger_range_, uint32_t explosion_range_)
		: Unit(false), ExplodingUnit(power_, explosion_range_), TriggeredUnit(trigger_range_)
		{
			setName("Mine");
		}

		[[nodiscard]] const std::vector<ActionTypeBase*>& getActionTypesOrder() const noexcept override
		{
			// Trigger is after Explode to ensure mine explodes on the next turn after triggering
			static const std::vector<ActionTypeBase*> types = {&ExplodeAttackActionType, &TriggerActionType};
			return types;
		}

		bool shouldExplodeNow() const noexcept override
		{
			return isTriggered();
		}

		void onActionExecuted(ActionTypeBase* actionType) override
		{
			Unit::onActionExecuted(actionType);
			// mine explodes and is destroyed
			// Action::actionType stores a pointer to the singleton ActionTypeBase instance.
			// Compare by pointer to the inline singleton object.
			if (actionType == &ExplodeAttackActionType)
			{
				setHp(0);
			}
		}
	};

} // namespace sw::core

