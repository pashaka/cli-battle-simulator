//
// Created by Carpov Pavel on 28.09.2025.
//

#ifndef SW_BATTLE_TEST_SPAWNMINE_HPP
#define SW_BATTLE_TEST_SPAWNMINE_HPP

#include <cstdint>

namespace sw::io
{
	struct SpawnMine
	{
		constexpr static const char* Name = "SPAWN_MINE";

		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};
		uint32_t power{};
		uint32_t triggerRange{};
		uint32_t explosionRange{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", x);
			visitor.visit("y", y);
			visitor.visit("power", power);
			visitor.visit("triggerRange", triggerRange);
			visitor.visit("explosionRange", explosionRange);
		}
	};
}
#endif	//SW_BATTLE_TEST_SPAWNMINE_HPP
