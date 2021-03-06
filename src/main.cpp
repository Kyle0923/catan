/**
 * Project: catan
 * @file main.cpp
 * @brief catan.exe entry point
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "common.hpp"
#include "user_interface.hpp"
#include "panel.h"
#include "game_map.hpp"
#include "map_file_io.hpp"
#include "cli_opt.hpp"
#include "command_dispatcher.hpp"
#include "command_handlers.hpp"
#include "trading_system.hpp"
#include "utility.hpp"
#include "logger.hpp"

int main(int argc, char** argv)
{
    Logger::initLogger();
    INFO_LOG("Begin of Catan.exe");

    CliOpt cliOpt;
    cliOpt.processArg(argc, argv);
    Logger::setDebugLevel(cliOpt.getOpt<CliOptIndex::DEBUG_LEVEL>());

    GameMap gameMap;

    {
        // auto release mapFile
        MapIO mapFile(cliOpt.getOpt<CliOptIndex::MAP_FILE_PATH>());
        mapFile.readMap(gameMap);
    }

    UserInterface ui(gameMap, std::make_unique<CommandDispatcher>(
        std::vector<CommandHandler*>({
            new BuildHandler(),
            new NextHandler(),
            new PassHandler(),
            new RollHandler(),
            new StatusHandler(),
            new DevelopmentCardHandler(),
            new TradeHandler(),
#ifndef RELEASE
            // these commands are only for testing in development
            new BuildingHandler(),
            new SubCmdHandler(),
            new ParameterExampleCommandHandler()
#endif /* ifndef RELEASE*/
        })
    ));

    gameMap.initMap();
    gameMap.logMap();
    gameMap.addPlayer(6U);

    ui.printMapToWindow(gameMap);

    const std::vector<int> playerOrder = gameMap.getFirstTwoRoundOrder();

    ui.loop(gameMap);

    INFO_LOG("Exiting Catan.exe...\nGoodbye");
    return 0;
}
