/**
 * Project: catan
 * @file main.cpp
 * @brief catan.exe entry point
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <iostream>
#include <fstream>
#include <time.h>
#include <string.h>
#include <cstdlib>
#include "logger.hpp"
#include "common.hpp"
#include "game_map.hpp"
#include "map_file_io.hpp"
#include "cli_opt.hpp"

#define DELAYSIZE 200

void myrefresh(void);
void get_color(void);
void explode(int, int);

short color_table[] =
{
    COLOR_RED, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN,
    COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE
};

int main(int argc, char** argv)
{
    Logger::initLogfile();
    // CliOpt_t cliOpt;
    // processOpt(argc, argv, cliOpt);
    // Logger::setDebugLevel(cliOpt.debugValue);
    CliOpt cliOpt;
    cliOpt.processArg(argc, argv);
    Logger::setDebugLevel(cliOpt.getOpt<CliOptIndex::DEBUG_LEVEL>());

    DEBUG_LOG_L0("Begin ", " of ", "main");
    GameMap gameMap;
    MapIO mapFile("map.txt");
    mapFile.readMap(gameMap);
    gameMap.initMap();
    gameMap.printMap();
    return 0;
    time_t seed;
    int start, end, row, diff, flag, direction;
    short i;

    initscr();
    keypad(stdscr, TRUE);  //return special keyboard stroke
    nodelay(stdscr, FALSE); // wait for input
    nocbreak(); // wait for new line

    if (has_colors())
        start_color();

    for (i = 0; i < 8; i++)
        init_pair(i, color_table[i], COLOR_BLACK);

    seed = time((time_t *)0);
    srand(seed);
    flag = 0;

    char line[1024];
    while (1)      /* loop until a key is hit */
    {
        getnstr(line, 2014);
        if (!strcmp(line, "quit"))
        {
            break;
        }
        do {
            start = rand() % (COLS - 3);
            end = rand() % (COLS - 3);
            start = (start < 2) ? 2 : start;
            end = (end < 2) ? 2 : end;
            direction = (start > end) ? -1 : 1;
            diff = abs(start - end);

        } while (diff < 2 || diff >= LINES - 2);

        attrset(A_NORMAL);

        for (row = 0; row < diff; row++)
        {
            mvaddstr(LINES - row, row * direction + start,
                (direction < 0) ? "\\" : "/");

            if (flag++)
            {
                myrefresh();
                erase();
                flag = 0;
            }
        }

        if (flag++)
        {
            myrefresh();
            flag = 0;
        }

        explode(LINES - row, diff * direction + start);
        erase();
        myrefresh();
    }

    endwin();

    return 0;
}

void explode(int row, int col)
{
    erase();
    mvaddstr(row, col, "-");
    myrefresh();

    --col;

    get_color();
    mvaddstr(row - 1, col, " - ");
    mvaddstr(row,     col, "-+-");
    mvaddstr(row + 1, col, " - ");
    myrefresh();

    --col;

    get_color();
    mvaddstr(row - 2, col, " --- ");
    mvaddstr(row - 1, col, "-+++-");
    mvaddstr(row,     col, "-+#+-");
    mvaddstr(row + 1, col, "-+++-");
    mvaddstr(row + 2, col, " --- ");
    myrefresh();

    get_color();
    mvaddstr(row - 2, col, " +++ ");
    mvaddstr(row - 1, col, "++#++");
    mvaddstr(row,     col, "+# #+");
    mvaddstr(row + 1, col, "++#++");
    mvaddstr(row + 2, col, " +++ ");
    myrefresh();

    get_color();
    mvaddstr(row - 2, col, "  #  ");
    mvaddstr(row - 1, col, "## ##");
    mvaddstr(row,     col, "#   #");
    mvaddstr(row + 1, col, "## ##");
    mvaddstr(row + 2, col, "  #  ");
    myrefresh();

    get_color();
    mvaddstr(row - 2, col, " # # ");
    mvaddstr(row - 1, col, "#   #");
    mvaddstr(row,     col, "     ");
    mvaddstr(row + 1, col, "#   #");
    mvaddstr(row + 2, col, " # # ");
    myrefresh();
}

void myrefresh(void)
{
    napms(DELAYSIZE);
    move(LINES - 1, COLS - 1);
    refresh();
}

void get_color(void)
{
    chtype bold = (rand() % 2) ? A_BOLD : A_NORMAL;
    attrset(COLOR_PAIR(rand() % 8) | bold);
}
