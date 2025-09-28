//
// Created by Carpov Pavel on 29.09.2025.
//

#ifndef SW_BATTLE_TEST_UNITHEALED_HPP
#define SW_BATTLE_TEST_UNITHEALED_HPP

namespace sw::io
{
	struct UnitHealed
	{
		constexpr static const char* Name = "UNIT_HEALED";

		uint32_t healerUnitId{};
		uint32_t targetUnitId{};
		uint32_t spirit{};
		uint32_t targetHp{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("healerUnitId", healerUnitId);
			visitor.visit("targetUnitId", targetUnitId);
			visitor.visit("spirit", spirit);
			visitor.visit("targetHp", targetHp);
		}
	};
}

#endif	//SW_BATTLE_TEST_UNITHEALED_HPP
