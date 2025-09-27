//
// Created by Carpov Pavel on 25.09.2025.
//
#pragma once

#include <Core/Engine/Coordinate.hpp>

#include <optional>
#include <vector>

namespace sw::core
{
	inline constexpr uint32_t MAX_MELEE_ATTACK_RANGE = 1;
	inline constexpr uint32_t MIN_RANGED_ATTACK_RANGE = 2;

	enum ActionType
	{
		MeleeAttackActionType,
		RangedAttackActionType, // single target
		ExplodeAttackActionType, // area of effect defined by range
		MoveActionType,
		HealActionType,
		WaitActionType, // do nothing, consume action
		LastActionType,
	};

	class Unit
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
		uint32_t strength{0};
		uint32_t agility{0}; // aka Power for towers
		uint32_t range_max{0};
		uint32_t range_min{2}; // min range for ranged attacks (1 = melee)

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
		virtual void setHp(int32_t hp_) { hp = std::max(hp_, 0); }
		[[nodiscard]] const std::optional<uint32_t>& getHp() const noexcept { return hp; }
		[[nodiscard]] bool hasHp() const noexcept { return hp.has_value(); }
		[[nodiscard]] bool isAlive() const noexcept { return hasHp() && hp.value() > 0; }
		[[nodiscard]] virtual bool canTakeDamage() const noexcept { return hasHp(); }
		[[nodiscard]] virtual bool canTakeMeleeDamage() const noexcept { return canTakeDamage(); }

		std::string getName() const noexcept { return name; }
		std::string getShortName() const noexcept { return name.substr(0, std::min(2, static_cast<int>(name.length()))); }
		void setName(const std::string& n) { name = n; }

		void setStrength(uint32_t v) noexcept { strength = v; }
		[[nodiscard]] uint32_t getStrength() const noexcept { return strength; }

		void setAgility(uint32_t v) noexcept { agility = v; }
		[[nodiscard]] uint32_t getAgility() const noexcept { return agility; }

		void setRange(uint32_t v) noexcept { range_max = v; }
		[[nodiscard]] uint32_t getRange() const noexcept { return range_max; }

		void setAvailableActionsPerTurn(uint32_t v) noexcept { availableActionsNum = v; }
		[[nodiscard]] uint32_t getAvailableActionsPerTurn() const noexcept { return availableActionsNum; }

		[[nodiscard]] bool canAttack() const noexcept { return strength > 0 || agility > 0; }

		[[nodiscard]] bool isSolid() const noexcept { return solid; }

		// Subclasses may override to return their allowed actions. Default is an empty list.
		virtual const std::vector<ActionType>& getActionTypes() const noexcept
		{
			static const std::vector<ActionType> empty{};
			return empty;
		}

		// Hook for unit-specific action restrictions that depend on game rules.
		virtual bool disallowRangedIfAdjacent() const noexcept { return false; }

		virtual uint32_t getDamageForActionType(ActionType actionType) const noexcept
		{
			uint32_t damage{0};
			switch (actionType)
			{
				case MeleeAttackActionType:
					damage = getStrength();
					break;
				case RangedAttackActionType:
					damage = getAgility();
					break;
				default:
					break;
			}
			return damage;
		}

		void consumeAction()
		{
			if (availableActionsNum == 0)
			{
				throw std::runtime_error("Unit::consumeAction: no available actions left");
			}
			--availableActionsNum;
		}

		static uint32_t getDefaultUnitActionsPerTurn() noexcept { return 1; }

		// callback hooks for actions
		virtual void onAfterAttack() {}
	};

	// abstract base for units that can move
	class MovingUnit : public Unit
	{
	private:
		// targetPosition is optional: unit may have no movement target
		std::optional<Coordinate> targetPosition{};
		uint32_t speed{1}; // tiles per move action

	public:
		explicit MovingUnit(bool solid_) : Unit(solid_) {}

		[[nodiscard]] uint32_t getSpeed() const noexcept { return speed; }
		[[nodiscard]] bool hasTarget() const noexcept { return targetPosition.has_value(); }
		[[nodiscard]] const std::optional<Coordinate>& getTarget() const noexcept { return targetPosition; }
		void setTarget(const Coordinate& coord) { targetPosition = coord; }
		void clearTarget() noexcept { targetPosition.reset(); }
	};

	class SwordsmanUnit : public MovingUnit
	{
	public:
		SwordsmanUnit() : MovingUnit(true) {}
		[[nodiscard]] const std::vector<ActionType>& getActionTypes() const noexcept override
		{
			static const std::vector<ActionType> types = {MeleeAttackActionType, MoveActionType};
			return types;
		}
	};

	class HunterUnit : public MovingUnit
	{
	public:
		HunterUnit() : MovingUnit(true) {}
		[[nodiscard]] const std::vector<ActionType>& getActionTypes() const noexcept override
		{
			static const std::vector<ActionType> types = {RangedAttackActionType, MeleeAttackActionType, MoveActionType};
			return types;
		}

		// Hunters restrict ranged attacks when adjacent (melee) units are present.
		bool disallowRangedIfAdjacent() const noexcept override { return true; }
	};

	class TowerUnit : public Unit
	{
	public:
		TowerUnit() : Unit(true) {}
		[[nodiscard]] const std::vector<ActionType>& getActionTypes() const noexcept override
		{
			static const std::vector<ActionType> types = {RangedAttackActionType};
			return types;
		}
	};

	class HealerUnit : public MovingUnit
	{
	public:
		HealerUnit() : MovingUnit(true) {}
		[[nodiscard]] const std::vector<ActionType>& getActionTypes() const noexcept override
		{
			static const std::vector<ActionType> types = {HealActionType, MoveActionType};
			return types;
		}
	};

	class MineUnit : public Unit
	{
		uint32_t triggerRange{2}; // range within which mine will explode
	public:
		MineUnit() : Unit(false)
		{
			range_max = 3; // explosion range
		}
		[[nodiscard]] const std::vector<ActionType>& getActionTypes() const noexcept override
		{
			static const std::vector<ActionType> types = {ExplodeAttackActionType};
			return types;
		}

		void onAfterAttack() override
		{
			// mine explodes and is destroyed
			setHp(0);
		}

		[[nodiscard]] uint32_t getTriggerRange() const noexcept { return triggerRange; }
	};

} // namespace sw::core
