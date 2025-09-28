//
// Created by Carpov Pavel on 25.09.2025.
//

#include "Engine.hpp"

#include <IO/Events/MapCreated.hpp>
#include <cassert>
#include <chrono>
#include <thread>

namespace sw::core
{
	std::unique_ptr<Engine> Engine::instance = nullptr;

	MapUnitsController* Engine::getMapUnitsController()
	{
		assert(battleMap && "Engine::getMapUnitsController: map not created");
		return battleMap.get();
	}

	void Engine::createMap(uint32_t w, uint32_t h)
    {
        // check if map already exists
        if (battleMap)
        {
            throw std::runtime_error("BattleMap already exists");
        }
        // pass EventLog reference and a callback to obtain current round/tick
        battleMap = std::make_unique<MapUnitsController>(w, h, eventLog, [this]() { return static_cast<uint64_t>(round); });
        // emit MapCreated event
        eventLog.log(round, sw::io::MapCreated{w, h});
    }

	void Engine::simulateRounds()
    {
	    while (true)
	    {
	    	getMapUnitsController()->handleNextRound();
	    	// advance round counter early, so the 1st round only has spawn events
	    	round++;
			getMapUnitsController()->removeDeadUnits();
	    	if (getMapUnitsController()->getUnitsCount()==1 || getMapUnitsController()->doTurn() == 0)
		    {
			    debugPrint("No actions performed in this round, ending simulation.");
			    break;
		    }

	    	// getMapUnitsController()->printMap();
		    // debugPrint(std::string("Starting round ") + std::to_string(round));

		    // avoid busy spin; sleep a short amount
		    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	    }
    }

    void Engine::handleCommand(const sw::io::CreateMap& cmd)
    {
        createMap(cmd.width, cmd.height);
    }

    void Engine::handleCommand(const sw::io::SpawnSwordsman& cmd)
    {
    	handleSpawn(cmd, std::string("Swordsman"));
    }

    void Engine::handleCommand(const sw::io::SpawnHunter& cmd)
    {
        handleSpawn(cmd, std::string("Hunter"));
    }

    void Engine::handleCommand(const sw::io::March& cmd /*cmd*/)
    {
    	getMapUnitsController()->assignMarchCommand(cmd.unitId, cmd.targetX, cmd.targetY);
    }

} // namespace sw::core
