/**
 * Project: catan
 * @file backdoor_handler.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_handlers.hpp"

std::string BackdoorHandler::command() const
{
    return "__";
}

ActionStatus BackdoorHandler::statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    // credit http://www.ascii-art.de/ascii/s/sharks.txt
    const std::string shark = \
    "\n"
    "                                   ,-\n"                                                 \
    "                                 ,'::|\n"                                                \
    "                                /::::|\n"                                                \
    "                              ,'::::o\\                                      _..\n"      \
    "           ____........-------,..::?88b                                  ,-' /\n"        \
    "   _.--\"\"\"\". . . .      .   .  .  .  \"\"`-._                           ,-' .;'\n"   \
    "  <. - :::::q......  ...   . . .. . .  .  .\"\"--._                  ,-'. .;'\n"         \
    "   `-._  ` `\":`:`:`::||||:::::::::::::::::.:. .  \"\"--._ ,'|     ,-'.  .;'\n"          \
    "       \"\"\"_=--       //'doo.. ````:`:`::::::::::.:.:.:. .`-`._-'.   .;'\n"            \
    "           \"\"--.__     P(       \\               ` ``:`:``:::: .   .;'\n"              \
    "                  \"\\\"\"--.:-.     `.                             .:/\n"               \
    "                    \\. /    `-._   `.\"\"-----.,-..::(--\"\".\\\"\"`.  `:\\\n"          \
    "                     `P         `-._ \\          `-:\\          `. `:\\\n"               \
    "                                     \"\"            \"            `-._)\n"              \
    ;

    // credit https://ascii.co.uk/art/dolphin
    // credit https://asciiart.website/index.php?art=plants/flowers
    const std::string dolphin = \
    "\n"
    "                                     __\n"          \
    "                                 _.-~  )\n"         \
    "               _ _    _..--~~~~,'   ,-/     _\n"    \
    "             _{ ' }_-'. . . .'   ,-','    ,' )\n"   \
    "            { `.!.` }. . _   ,--~,-'__..-'  ,'\n"   \
    "            ',_/Y\\_,'.  (@)' ---~~~~      ,'\n"    \
    "              {_,_}. . '~~             ,-'\n"       \
    "             /. . . . .             ,-'\n"          \
    "            ; . . . .  - .        ,'\n"             \
    "           : . . . .       _     /\n"               \
    "          . . . . .          `-.:\n"                \
    "         . . . ./  - .          )\n"                \
    "        .  . . |  _____..---.._/ \n"                \
    "  ~---~~~~----~~~~             ~~\n"                \
    ;

    // credit https://textart.sh/topic/seal
    const std::string seal = \
    "\n"
    "                                                WWWWWW\n"                                                                           \
    "                                                WW  WWWWWW\n"                                                                       \
    "                                                WWWWWW**WW\n"                                                                       \
    "                            @@@@@@@@@@@@WW      WW**WW@@WW@@\n"                                                                     \
    "                    WW@@@@@@**************@@@@@@WW@@WWWW**WW\n"                                                                     \
    "                WW##********              ********WWWWWW##WW\n"                                                                     \
    "              WW****                              WWWWWW  WW\n"                                                                     \
    "            ##                                    WWWWWW    ####\n"                                                                 \
    "          WW                                      WWWWWW        ####\n"                                                             \
    "          WW                                      WWWWWW            ##\n"                                                           \
    "        WW                                        WWWWWW              WWWWWWWW\n"                                                   \
    "        WW                                        WWWWWW                      WWWW\n"                                               \
    "      WW                                          WWWWWW                          WW\n"                                             \
    "    WW                                            WWWWWW                            WW                  WWWWWWWWWW\n"               \
    "    WW      *WWWW*                                WWWWWW                              WWWWWWWWWWWW  WWWWWW        WWWW\n"           \
    "    WW      WWWWWW                                WWWWWW                              ************@@WW****          WW\n"           \
    "  @@**      WWWWWW          $@@@@$                WWWWWW                                          ****              WW\n"           \
    "  WW        WWWWWW          WWWWWW                WWWWWW                                                        WWWWWW\n"           \
    "  WW        *WWWW*          WWWWWW                WWWWWW                                                      WW\n"                 \
    "  WW                        WWWWWW                WWWWWW                                                    WWWW######\n"           \
    "  WW                        *WWWW*                WWWWWW                                                  WW        WW##\n"         \
    "  WW            WW####                            WWWWWW                                                              WW##\n"       \
    "    WW      ####WWWWWW####                        WWWWWW          WW                                                    WW\n"       \
    "    WW          WWWWWW                            WWWWWW          WWWW                                                  WW\n"       \
    "      WW                                          WWWWWW            WW                                              WWWWWW\n"       \
    "        WWWWWW                                    WWWWWW            WWWW                                        WWWW\n"             \
    "              WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW              WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n"                 \
    "                **************************************WW@@            WW  ************************************\n"                   \
    "                                                        WWWW        WWWW\n"                                                         \
    "                                                        **WW@@    @@WW**\n"                                                         \
    "                                                          **WW@@@@WW**\n"                                                           \
    ;

    // credit https://www.asciiart.eu/animals/cats
    const std::string cat = \
    "\n"
    "              __..--''``---....___   _..._    __\n"           \
    "          _.-'    .-/\";  `        ``<._  ``.''_ `.\n"        \
    "  (@) _.-' _..--.'_    \\                    `( ) )\n"        \
    "     (_..-'    (< _     ;_..__               ; `'\n"          \
    "                `-._,_)'      ``--...____..-'\n"              \
    ;

    // credit https://www.asciiart.eu/animals/dogs
    const std::string dog = \
    "\n"
    "     _=,_\n"             \
    "  o_/6 /#\\\n"           \
    "  \\__ |##/\n"           \
    "   ='|--\\\n"            \
    "     /   #'-.\n"         \
    "     \\#|_   _'-. /\n"   \
    "      |/ \\_( # |\"\n"   \
    "     C/ ,--___/\n"       \
    ;

    if (aArgs.size() > 0)
    {
        if (aArgs.front() == "shark") {
            aReturnMsg.push_back(shark);
        } else if (aArgs.front() == "dolphin") {
            aReturnMsg.push_back(dolphin);
        } else if (aArgs.front() == "seal") {
            aReturnMsg.push_back(seal);
        } else if (aArgs.front() == "cat") {
            aReturnMsg.push_back(cat);
        } else if (aArgs.front() == "dog") {
            aReturnMsg.push_back(dog);
        }
        return ActionStatus::SUCCESS;
    }
    else
    {
        aReturnMsg.push_back("parameter required");
        return ActionStatus::PARAM_REQUIRED;
    }
}
