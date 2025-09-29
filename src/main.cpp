#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/SpawnSwordsman.hpp"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Commands/SpawnMine.hpp"
#include "IO/Commands/SpawnHealer.hpp"

#include <Core/Engine/Engine.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/PrintDebug.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>

int main(int argc, char** argv)
{
    using namespace sw;

    if (argc != 2)
    {
        throw std::runtime_error("Error: No file specified in command line argument");
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
    }

    std::cout << "Commands:\n";
    io::CommandParser parser;
    core::Engine engine;

    // Collect parsed commands and execute them after printing Events
    std::vector<std::function<void()>> pendingCommands;

    // For each command type: print it during parse, but defer execution
    parser.add<io::CreateMap>([&pendingCommands, &engine](auto command) {
        printDebug(std::cout, command);
        pendingCommands.push_back([&engine, c = std::move(command)]() mutable {
            engine.handleCommand(std::move(c));
        });
    })
    .add<io::SpawnSwordsman>([&pendingCommands, &engine](auto command) {
        printDebug(std::cout, command);
        pendingCommands.push_back([&engine, c = std::move(command)]() mutable {
            engine.handleCommand(std::move(c));
        });
    })
    .add<io::SpawnHunter>([&pendingCommands, &engine](auto command) {
        printDebug(std::cout, command);
        pendingCommands.push_back([&engine, c = std::move(command)]() mutable {
            engine.handleCommand(std::move(c));
        });
    })
    .add<io::SpawnMine>([&pendingCommands, &engine](auto command) {
        printDebug(std::cout, command);
        pendingCommands.push_back([&engine, c = std::move(command)]() mutable {
            engine.handleCommand(std::move(c));
        });
    })
    .add<io::SpawnHealer>([&pendingCommands, &engine](auto command) {
        printDebug(std::cout, command);
        pendingCommands.push_back([&engine, c = std::move(command)]() mutable {
            engine.handleCommand(std::move(c));
        });
    })
    .add<io::March>([&pendingCommands, &engine](auto command) {
        printDebug(std::cout, command);
        pendingCommands.push_back([&engine, c = std::move(command)]() mutable {
            engine.handleCommand(std::move(c));
        });
    });

    // Parse file: handlers will print commands and enqueue deferred actions
    parser.parse(file);

    std::cout << "\n\nEvents:\n";

    // Now execute deferred commands (they will emit events which should be printed under Events)
    for (auto &fn : pendingCommands) {
        fn();
    }

    // Run simulation after commands applied
    engine.simulateRounds();

    return 0;
}
