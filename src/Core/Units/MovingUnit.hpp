//
// Created by Carpov Pavel on 28.09.2025.
//

#ifndef SW_BATTLE_TEST_MOVINGUNIT_HPP
#define SW_BATTLE_TEST_MOVINGUNIT_HPP
#include "Core/Engine/Action.hpp"
#include "Unit.hpp"

namespace sw::core
{
	inline constexpr uint32_t MIN_MOVE_RANGE = 1;
	// abstract base for units that can move
	class MovingUnit : public virtual Unit
	{
		friend class MoveActionTypeClass;
	private:
		// targetPosition is optional: unit may have no movement target
		std::optional<Coordinate> targetPosition{};
		uint32_t speed{1}; // tiles per move action

	public:
		explicit MovingUnit() {}

		[[nodiscard]] uint32_t getSpeed() const noexcept { return speed; }
		[[nodiscard]] bool hasTarget() const noexcept { return targetPosition.has_value(); }
		[[nodiscard]] const std::optional<Coordinate>& getTarget() const noexcept { return targetPosition; }
		void setTarget(const Coordinate& coord) { targetPosition = coord; }
		void clearTarget() noexcept { targetPosition.reset(); }
	protected:
		bool tryToExecuteMove(const MapUnitsController& worldState);
	};

	inline class MoveActionTypeClass : public ActionTypeBase
	{
	public:
		bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) override
		{
			if (auto movingUnit = std::dynamic_pointer_cast<MovingUnit>(unit))
			{
				return movingUnit->tryToExecuteMove(worldState);
			}
			return false; // no action performed
		}
	} MoveActionType;

}

#endif	//SW_BATTLE_TEST_MOVINGUNIT_HPP
