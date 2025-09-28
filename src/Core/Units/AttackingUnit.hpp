//
// Created by Carpov Pavel on 28.09.2025.
//
#ifndef SW_BATTLE_TEST_ATTACKINGUNIT_HPP
#define SW_BATTLE_TEST_ATTACKINGUNIT_HPP
#include "Core/Engine/Action.hpp"
#include "Core/Engine/MapUnitsController.hpp"
#include "Unit.hpp"

#include <cstdint>

namespace sw::core
{
	inline constexpr uint32_t MAX_MELEE_ATTACK_RANGE = 1;
	inline constexpr uint32_t MIN_MELEE_ATTACK_RANGE = 1;
	class MeleeAttackingUnit : public virtual Unit
	{
		friend class MeleeAttackActionTypeClass;
		uint32_t strength{0};
	public:
		explicit MeleeAttackingUnit(uint32_t strength_)
		{
			strength = strength_;
		}
		void setStrength(uint32_t v) noexcept { strength = v; }
		[[nodiscard]] uint32_t getStrength() const noexcept { return strength; }
		// helper used by Unit::decideNextActionOfType dispatch
	protected:
		bool tryToExecuteMeleeAttack(const MapUnitsController& worldState);
	};

	inline constexpr uint32_t MIN_RANGED_ATTACK_RANGE = 2;
	class RangedAttackingUnit : public virtual Unit
	{
		friend class RangedAttackActionTypeClass;
	private:
		uint32_t agility{0}; // aka Power for towers
		uint32_t range_max{0};
		uint32_t range_min{2}; // min range for ranged attacks (1 = melee)
	public:
		RangedAttackingUnit(uint32_t agility_, uint32_t range_min_, uint32_t range_max_)
		{
			range_max = range_max_;
			range_min = range_min_;
			agility = agility_;
		}

		void setAgility(uint32_t v) noexcept { agility = v; }
		[[nodiscard]] uint32_t getAgility() const noexcept { return agility; }

		void setRange(uint32_t v) noexcept { range_max = v; }
		[[nodiscard]] uint32_t getRange() const noexcept { return range_max; }

		// Hook for unit-specific action restrictions that depend on game rules.
		[[nodiscard]] virtual bool disallowRangedIfAdjacent() const noexcept { return false; }
	protected:
		bool tryToExecuteRangedAttack(const MapUnitsController& worldState) const;
	};

	inline class MeleeAttackActionTypeClass : public ActionTypeBase
	{
	public:
		bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) override
		{
			if (auto meleeUnit = std::dynamic_pointer_cast<MeleeAttackingUnit>(unit))
			{
				return meleeUnit->tryToExecuteMeleeAttack(worldState);
			}
			return false; // no action performed
		}
	} MeleeAttackActionType;

	inline class RangedAttackActionTypeClass : public ActionTypeBase
	{
	public:
		bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) override
		{
			if (auto meleeUnit = std::dynamic_pointer_cast<RangedAttackingUnit>(unit))
			{
				return meleeUnit->tryToExecuteRangedAttack(worldState);
			}
			return false; // no action performed
		}
	} RangedAttackActionType;
}

#endif	//SW_BATTLE_TEST_ATTACKINGUNIT_HPP
