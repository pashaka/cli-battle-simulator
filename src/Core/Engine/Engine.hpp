//
// Created by Carpov Pavel on 25.09.2025.
//

#ifndef SW_BATTLE_TEST_ENGINE_HPP
#define SW_BATTLE_TEST_ENGINE_HPP

#include "IO/Commands/SpawnHealer.hpp"
#include "IO/Commands/SpawnMine.hpp"
#include "IO/Events/UnitSpawned.hpp"
#include "MapUnitsController.hpp"

#include <Core/Units/Unit.hpp>
#include <Core/Units/UnitFactory.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/March.hpp>
#include <IO/System/EventLog.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

namespace sw::core
{
	// Singleton Engine class that processes commands and manages game state
    class Engine
    {
    	static std::unique_ptr<Engine> instance;
    public:
    	static Engine* getInstance()
    	{
    		if (!instance)
			{
				instance.reset(new Engine());
			}
    		return instance.get();
    	}

    	~Engine() = default;

        // Command handlers
        void handleCommand(const sw::io::CreateMap& cmd);
        void handleCommand(const sw::io::SpawnSwordsman& cmd);
        void handleCommand(const sw::io::SpawnHunter& cmd);
        void handleCommand(const sw::io::SpawnMine& cmd);
        void handleCommand(const sw::io::SpawnHealer& cmd);
        void handleCommand(const sw::io::March& cmd);

    	void simulateRounds();
    private:
		std::unique_ptr<MapUnitsController> battleMap;
		uint32_t round{1};

    	MapUnitsController* getMapUnitsController();

        void debugPrint(const std::string& msg) const { std::cout << msg << std::endl; }
    	void createMap(uint32_t width, uint32_t height);

		sw::EventLog eventLog; // log/emitter for produced events

        template <typename TCommand>
        void handleSpawn(const TCommand& cmd, const std::string& unitType)
        {
            auto unit = UnitFactory::create(cmd);
            getMapUnitsController()->placeUnit(std::move(unit));

            eventLog.log(round, sw::io::UnitSpawned{cmd.unitId, unitType, cmd.x, cmd.y});
        }
    };
}

#endif //SW_BATTLE_TEST_ENGINE_HPP
