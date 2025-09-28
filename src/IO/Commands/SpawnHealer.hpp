//
// Created by Carpov Pavel on 29.09.2025.
//

#ifndef SW_BATTLE_TEST_SPAWNHEALER_HPP
#define SW_BATTLE_TEST_SPAWNHEALER_HPP

#include <cstdint>

namespace sw::io
{
	struct SpawnHealer
	{
		constexpr static const char* Name = "SPAWN_HEALER";

		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};
		uint32_t hp{};
		uint32_t spirit{};
		uint32_t healRange{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", x);
			visitor.visit("y", y);
			visitor.visit("hp", hp);
			visitor.visit("spirit", spirit);
			visitor.visit("healRange", healRange);
		}
	};
}


#endif	//SW_BATTLE_TEST_SPAWNHEALER_HPP
