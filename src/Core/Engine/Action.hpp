//
// Created by Carpov Pavel on 26.09.2025.
//

#ifndef SW_BATTLE_TEST_ACTION_HPP
#define SW_BATTLE_TEST_ACTION_HPP
#include "Coordinate.hpp"

#include <memory>

namespace sw::core
{
	class Unit;
	class MapUnitsController;

	class ActionTypeBase
	{
	public:
		virtual ~ActionTypeBase() = default;
		virtual bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) = 0;
	};

	inline class WaitActionTypeClass : public ActionTypeBase
	{
	public:
		bool tryToExecute(const std::shared_ptr<Unit>& unit, const MapUnitsController& worldState) override;
	} WaitActionType;

}
#endif	//SW_BATTLE_TEST_ACTION_HPP
