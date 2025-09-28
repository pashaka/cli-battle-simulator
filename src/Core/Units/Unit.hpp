//
// Created by Carpov Pavel on 25.09.2025.
//
#pragma once

#include <Core/Engine/Coordinate.hpp>
#include <cassert>
#include <optional>
#include <vector>

namespace sw::core
{
	class ActionTypeBase;
	class MapUnitsController;
	class Action;
	// inline constexpr uint32_t MIN_RANGED_ATTACK_RANGE = 2;

	inline constexpr uint32_t DEFAULT_ACTIONS_PER_TURN = 1;

	class Unit : public std::enable_shared_from_this<Unit>
	{
	private:
		std::string name;
		Coordinate position{};
		// identity and common stats
		uint32_t id{0};
		std::optional<uint32_t> hp; // optional HP
		uint32_t availableActionsNum{0}; // number of actions available in the current round
		const bool solid{true}; // Fermion / Boson :) other units can move through this unit

	protected:

	public:
		// Unit no longer stores per-instance action lists. Subclasses provide them via getActionTypes().
		explicit Unit(bool solid_) : solid(solid_) {}
		virtual ~Unit() = default;

		[[nodiscard]] const Coordinate& getPosition() const noexcept { return position; }
		void setPosition(const Coordinate& pos) noexcept { position = pos; }

		// identity / stats accessors
		void setId(uint32_t uid) noexcept { id = uid; }
		[[nodiscard]] uint32_t getId() const noexcept { return id; }

		// HP is optional
		[[nodiscard]] const std::optional<uint32_t>& getHp() const noexcept { return hp; }
		virtual void setHp(int32_t hp_) { hp = std::max(hp_, 0); }
		virtual void increaseHp(int32_t delta)
		{
			assert(hp.has_value() && "Unit::increaseHp: unit has no HP");
			if (hp) setHp(hp.value() + delta);
		}
		[[nodiscard]] bool hasHp() const noexcept { return hp.has_value(); }
		[[nodiscard]] bool isAlive() const noexcept { return !hasHp() || hp.value() > 0; }
		[[nodiscard]] virtual bool canTakeDamage() const noexcept { return hasHp() && hp.value() > 0; }
		[[nodiscard]] virtual bool canTakeMeleeDamage() const noexcept { return canTakeDamage(); }
		[[nodiscard]] virtual bool canTakeRangedDamage() const noexcept { return canTakeDamage(); }

		std::string getName() const noexcept { return name; }
		std::string getShortName() const noexcept { return name.substr(0, std::min(2, static_cast<int>(name.length()))); }
		void setName(const std::string& n) { name = n; }

		void setAvailableActionsPerTurn(uint32_t v) noexcept { availableActionsNum = v; }
		[[nodiscard]] uint32_t getAvailableActionsPerTurn() const noexcept { return availableActionsNum; }

		[[nodiscard]] bool isSolid() const noexcept { return solid; }

		// Subclasses may override to return their allowed actions. Default is an empty list.
		[[nodiscard]] virtual const std::vector<ActionTypeBase*>& getActionTypesOrder() const noexcept = 0;

		void consumeAction()
		{
			assert(availableActionsNum > 0 && "Unit::consumeAction: no available actions left");
			--availableActionsNum;
		}

		static uint32_t getDefaultUnitActionsPerTurn() noexcept { return DEFAULT_ACTIONS_PER_TURN; }

		// callback hooks for actions
		[[nodiscard]] bool tryToExecuteNextAction(const MapUnitsController& worldState);
		virtual void onActionExecuted(ActionTypeBase*) {}
	};

	// Concrete unit types (Swordsman, Hunter, Tower, Healer) moved to UnitTypes.hpp to avoid
	// circular includes and construction-order problems.

} // namespace sw::core
