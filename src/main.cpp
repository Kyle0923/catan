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
#include "cli_command_manager.hpp"
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
    MapIO mapFile(cliOpt.getOpt<CliOptIndex::MAP_FILE_PATH>());
    mapFile.readMap(gameMap);

    UserInterface ui(gameMap, std::make_unique<CliCommandManager>(  \
        std::vector<CommandHandler*>({new SubCmdHandler()})         \
    ));

    gameMap.initMap();
    gameMap.logMap();

    ui.printMapToWindow(gameMap);

    ui.loop(gameMap);

    INFO_LOG("Exiting Catan.exe...\nGoodbye");
    return 0;
}
