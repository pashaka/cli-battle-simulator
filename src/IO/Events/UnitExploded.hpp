//
// Created by Carpov Pavel on 29.09.2025.
//

#ifndef SW_BATTLE_TEST_UNIT_EXPLODED_HPP
#define SW_BATTLE_TEST_UNIT_EXPLODED_HPP
#include <cstdint>

namespace sw::io
{
	struct UnitExploded
	{
		constexpr static const char* Name = "UNIT_EXPLODED";

		uint32_t unitId{};
		uint32_t power{};
		uint32_t hitUnits{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("power", power);
			visitor.visit("hitUnits", hitUnits);
		}
	};
}

#endif	//SW_BATTLE_TEST_UNIT_EXPLODED_HPP
