//
// Created by Carpov Pavel on 28.09.2025.
//

#ifndef SW_BATTLE_TEST_TRIGGEREDUNIT_HPP
#define SW_BATTLE_TEST_TRIGGEREDUNIT_HPP
#include "Core/Engine/Action.hpp"
#include "Unit.hpp"

namespace sw::core
{
	inline constexpr uint32_t DEFAULT_TRIGGER_RANGE = 2;
	// abstract base for units that can move
	class TriggeredUnit : public virtual Unit
	{
		friend class TriggerActionTypeClass;
	private:
		bool triggered{false};
		uint32_t triggerRange{DEFAULT_TRIGGER_RANGE};
	public:
		explicit TriggeredUnit(uint32_t triggerRange_) : triggerRange(triggerRange_) {};

		[[nodiscard]] bool isTriggered() const noexcept { return triggered; }
	protected:
		bool tryToExecuteTrigger(const MapUnitsController& worldState);
	};

	inline class TriggerActionTypeClass : public ActionTypeBase
	{
	public:
		bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) override
		{
			if (auto unit_ = std::dynamic_pointer_cast<TriggeredUnit>(unit))
			{
				return unit_->tryToExecuteTrigger(worldState);
			}
			return false; // no action performed
		}
	} TriggerActionType;

}

#endif	//SW_BATTLE_TEST_TRIGGEREDUNIT_HPP
