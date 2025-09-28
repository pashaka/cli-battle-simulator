//
// Created by Carpov Pavel on 28.09.2025.
//

#ifndef SW_BATTLE_TEST_EXPLODINGUNIT_HPP
#define SW_BATTLE_TEST_EXPLODINGUNIT_HPP
#include "Core/Engine/Action.hpp"
#include "Core/Engine/MapUnitsController.hpp"
#include "Unit.hpp"

namespace sw::core
{
	// abstract base for units that can move
	class ExplodingUnit : public virtual Unit
	{
		friend class ExplodeAttackActionTypeClass;
	private:
		uint32_t explosionRange{1};
		uint32_t power{1}; // damage dealt by explosion
	public:
		ExplodingUnit(uint32_t power_, uint32_t explosionRange_) : power(power_), explosionRange(explosionRange_) {};

	protected:
		virtual bool shouldExplodeNow() const noexcept { return false; } // override for custom logic
		bool tryToExecuteExplosion(const MapUnitsController& worldState);
	};

	inline class ExplodeAttackActionTypeClass : public ActionTypeBase
	{
	public:
		bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) override
		{
			if (auto unit_ = std::dynamic_pointer_cast<ExplodingUnit>(unit))
			{
				return unit_->tryToExecuteExplosion(worldState);
			}
			return false; // no action performed
		}
	} ExplodeAttackActionType;

}
#endif	//SW_BATTLE_TEST_EXPLODINGUNIT_HPP
