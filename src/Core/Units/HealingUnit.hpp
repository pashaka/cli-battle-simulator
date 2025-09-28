//
// Created by Carpov Pavel on 28.09.2025.
//

#ifndef SW_BATTLE_TEST_HEALINGUNIT_HPP
#define SW_BATTLE_TEST_HEALINGUNIT_HPP
#include "Core/Engine/Action.hpp"
#include "Unit.hpp"

namespace sw::core
{
	class HealingUnit : public virtual Unit
	{
		friend class HealActionTypeClass;
	private:
		uint32_t healingRange{1};
		uint32_t spirit{1}; // damage dealt by explosion
	public:
		HealingUnit(uint32_t spirit_, uint32_t healingRange_) : spirit(spirit_), healingRange(healingRange_) {};

	protected:
		bool tryToExecuteHeal(const MapUnitsController& worldState);
	};

	inline class HealActionTypeClass : public ActionTypeBase
	{
	public:
		bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) override
		{
			if (auto unit_ = std::dynamic_pointer_cast<HealingUnit>(unit))
			{
				return unit_->tryToExecuteHeal(worldState);
			}
			return false; // no action performed
		}
	} HealActionType;
}

#endif	//SW_BATTLE_TEST_HEALINGUNIT_HPP
