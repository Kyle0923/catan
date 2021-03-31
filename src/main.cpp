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

short available[] =
{
    COLOR_RED, COLOR_BLUE, COLOR_YELLOW, COLOR_MAGENTA, COLOR_WHITE
};

int main(int argc, char** argv)
{
    Logger::initLogfile();
    DEBUG_LOG_L0("Begin of Catan.exe");

    CliOpt cliOpt;
    cliOpt.processArg(argc, argv);
    Logger::setDebugLevel(cliOpt.getOpt<CliOptIndex::DEBUG_LEVEL>());

    GameMap gameMap;
    MapIO mapFile("map.txt");
    mapFile.readMap(gameMap);
    // gameMap.initMap();
    // gameMap.printMap();

    time_t seed;
    int start, end, row, diff, flag, direction;

    initscr();
    keypad(stdscr, TRUE);  //return special keyboard stroke
    nodelay(stdscr, FALSE); // wait for input
    nocbreak(); // wait for new line

    // alias stdscr to userWindow, for consistence and readability
    WINDOW* & userWindow = stdscr;
    resize_window(userWindow, LINES - gameMap.getSizeVertical() - 1, COLS);
    mvwin(userWindow, gameMap.getSizeVertical() + 1, 0);

    WINDOW* gameWindow = newwin(gameMap.getSizeVertical(), gameMap.getSizeHorizontal(), 0, 0);
    // WINDOW* userWindow = newwin(LINES - gameMap.getSizeVertical() - 1, 0, gameMap.getSizeVertical() + 1, 0);

    if (has_colors())
    {
        start_color();
    }

    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_BLUE, COLOR_BLUE);
    init_pair(4, COLOR_YELLOW, COLOR_YELLOW);

    wbkgd(gameWindow, COLOR_PAIR(1));
    wbkgd(userWindow, COLOR_PAIR(2));

    wborder(gameWindow, COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3));
    wborder(userWindow, COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4));

    mvwaddch(userWindow, 1, 1, '>');
    wrefresh(gameWindow);
    wrefresh(userWindow);

    seed = time((time_t *)0);
    srand(seed);
    flag = 0;
    gameMap.initMap();
    gameMap.printMap();
    char line[1024];
    int ctr = 0;
    std::string numStr = "/-\\|";
    while (1)      /* loop until a key is hit */
    {
        getnstr(line, 2014);
        if (!strcmp(line, "quit") || !strcmp(line, "exit"))
        {
            break;
        }
        gameMap.printMap(gameWindow);
        mvwaddch(gameWindow, 1, 1, COLOR_PAIR(1) | numStr.at(ctr++ % numStr.length()));
        // clear user window, reset cursor position
        wclear(userWindow);
        wborder(gameWindow, COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3), COLOR_PAIR(3));
        wborder(userWindow, COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4), COLOR_PAIR(4));
        mvwaddch(userWindow, 1, 1, '>');
        wrefresh(gameWindow);
        wrefresh(userWindow);
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
