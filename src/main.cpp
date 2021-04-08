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
#include "common.hpp"
#include "panel.h"
#include "logger.hpp"
#include "game_map.hpp"
#include "map_file_io.hpp"
#include "cli_opt.hpp"
#include "cli_command_manager.hpp"
#include "utility.hpp"

enum ColorIndex
{
    RESERVED = 0,
    GAME_WIN_BACKGROUND,
    USER_WIN_BACKGROUND,
    GAME_WIN_BORDER,
    USER_WIN_BORDER
};

void printToConsole(PANEL* const aPanel, const std::vector<std::string>& aMsg, bool aIsList, size_t aNormalSize = 0);
int readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, std::string& aString);
void printBorder(WINDOW* const aWindow, const chtype aColor);
void checkSize(const GameMap& aMap, const int aBottomPadding, const int aRightPadding);
void refreshAll(const GameMap& aMap, WINDOW* const aUserWindow, WINDOW* const aGameWindow, PANEL* const aPrintPanel);

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

    initscr();

    // check size
    checkSize(gameMap, 10, 10);

    WINDOW* gameWindow = newwin(gameMap.getSizeVertical(), gameMap.getSizeHorizontal(), 0, 0);
    PANEL* gamePanel = new_panel(gameWindow);
    move_panel(gamePanel, 0, 0);
    top_panel(gamePanel);

    // user panel
    WINDOW* userWindow = newwin(LINES - gameMap.getSizeVertical() - 1, 0, 0, 0);
    PANEL* userPanel = new_panel(userWindow);
    move_panel(userPanel, gameMap.getSizeVertical() + 1, 0);
    bottom_panel(userPanel);

    // printout panel
    WINDOW* printoutWindow = newwin(LINES - gameMap.getSizeVertical() - 4, COLS - 2, 0, 0);
    PANEL* printoutPanel = new_panel(printoutWindow);
    move_panel(printoutPanel, gameMap.getSizeVertical() + 3, 1);
    bottom_panel(printoutPanel);
    update_panels();

    keypad(userWindow, TRUE);  //return special keyboard stroke
    nocbreak();
    raw();      //capture ctrl-C etc...
    noecho();
    wtimeout(userWindow, 200);  //read input timeout 0.2sec

    if (has_colors())
    {
        start_color();
    }

    init_pair(ColorIndex::GAME_WIN_BACKGROUND, COLOR_BLACK, COLOR_CYAN);
    init_pair(ColorIndex::USER_WIN_BACKGROUND, COLOR_BLACK, COLOR_WHITE);
    init_pair(ColorIndex::GAME_WIN_BORDER, COLOR_BLUE, COLOR_BLUE);
    init_pair(ColorIndex::USER_WIN_BORDER, COLOR_YELLOW, COLOR_YELLOW);

    wbkgd(gameWindow, COLOR_PAIR(ColorIndex::GAME_WIN_BACKGROUND));
    wbkgd(userWindow, COLOR_PAIR(ColorIndex::USER_WIN_BACKGROUND));
    wbkgd(printoutWindow, COLOR_PAIR(ColorIndex::USER_WIN_BACKGROUND));

    mvwaddch(userWindow, 1, 1, '>');
    wrefresh(userWindow);
    wrefresh(gameWindow);

    gameMap.initMap();
    gameMap.printMap(gameWindow);

    printBorder(gameWindow, COLOR_PAIR(ColorIndex::GAME_WIN_BORDER));
    printBorder(userWindow, COLOR_PAIR(ColorIndex::USER_WIN_BORDER));
    wrefresh(userWindow);
    wrefresh(gameWindow);
    update_panels();
    doupdate();

    CliCommandManager cmdHandler;
    cmdHandler.init();

    // infinite loop preperation
    int spinCtr = 0;
    std::string spinner = "/-\\|";
    std::string input = "";
    int keystroke;
    mvwaddch(userWindow, 1, 1, '>');
    while (1)      /* loop until a key is hit */
    {
        keystroke = wgetch(userWindow);
        if (keystroke == ERR)
        {
            mvwaddch(gameWindow, 1, 1, spinner[++spinCtr % 4]);
            wrefresh(gameWindow);
            continue;
        }

        DEBUG_LOG_L0("keystroke recorded: ", (char)keystroke, " int: ", keystroke);
        switch (keystroke){
            // pdcurses resize does not work
            // case KEY_RESIZE:
            // case KEY_F(5):
            // {
            //     resize_term(0, 0);
            //     refreshAll(gameMap, userWindow, gameWindow, printoutPanel);
            //     continue;
            // }
            case 27: // esc
            {
                break;
            }
            case 3:  // ASCII 3 ctrl-C
            case 4:  // ASCII 4 ctrl-D
            {
                wmove(userWindow, 1, 2);
                wclrtoeol(userWindow);
                break;
            }
            case KEY_BACKSPACE:
            case 8:   // ASCII 8 is backspace
            {
                int curX, curY;
                getyx(userWindow, curY, curX);
                if (curX > 2)
                {
                    wmove(userWindow, curY, curX - 1);
                    wdelch(userWindow);
                }
                break;
            }
            case KEY_DC: //delete
            {
                wdelch(userWindow);;
                break;
            }

            case KEY_LEFT:
            {
                int curX, curY;
                getyx(userWindow, curY, curX);
                wmove(userWindow, curY, curX - 1);
                break;
            }
            case KEY_RIGHT:
            {
                int curX, curY;
                getyx(userWindow, curY, curX);
                char currPos = static_cast<char>(mvwinch(userWindow, curY, curX));
                char nextPos = static_cast<char>(mvwinch(userWindow, curY, curX + 1));
                if (nextPos == ' ' && currPos == ' ')
                {
                    DEBUG_LOG_L2("Key Right reached EOL");
                    // reached eol, put cursor back
                    wmove(userWindow, curY, curX);
                }
                break;
            }
            case KEY_HOME:
            case KEY_PPAGE:
            {
                wmove(userWindow, 1, 2);
                break;
            }
            case KEY_END:
            case KEY_NPAGE:
            {
                std::string str;
                int curX = readStringFromWindow(userWindow, 1, 1, true, str);
                wmove(userWindow, 1, curX);
                break;
            }
            case ' ':
            {
                int curX, curY;
                getyx(userWindow, curY, curX);
                char prevPos = static_cast<char>(mvwinch(userWindow, curY, curX - 1));
                char currPos = static_cast<char>(mvwinch(userWindow, curY, curX));
                if (prevPos == ' ' && currPos == ' ')
                {
                    // discard
                }
                else
                {
                    break;
                }
            }
            case '\t':
            {
                // auto fill
                input.clear();
                readStringFromWindow(userWindow, 1, 2, false, input);
                std::string autoFillString;
                std::vector<std::string> matched = cmdHandler.commandMatcher(input, &autoFillString);
                DEBUG_LOG_L0("match size == ", matched.size());
                if (matched.size() == 1)
                {
                    // overwrite the longestCommonStr to the matched string
                    autoFillString = matched.at(0);
                }
                if (autoFillString.length() > input.size())
                {
                    mvwaddnstr(userWindow, 1, 2, autoFillString.c_str(), autoFillString.length());
                    wmove(userWindow, 1, 2 + autoFillString.length());
                }
                break;
            }
        }

        if (isprint((char)keystroke) && keystroke <= 126)
        {
            // echo to userWindow, advance cursor
            winsch(userWindow, keystroke);
            printBorder(userWindow, COLOR_PAIR(ColorIndex::USER_WIN_BORDER)); // winsch will remove the border (char) at the end of current line
            int curX, curY;
            getyx(userWindow, curY, curX);
            wmove(userWindow, curY, curX + 1);
        }
        input.clear();
        readStringFromWindow(userWindow, 1, 2, false, input);
        std::vector<std::string> matchedCmd = cmdHandler.commandMatcher(input);
        printToConsole(printoutPanel, matchedCmd, true, input.length());

        if (!(keystroke == KEY_ENTER || keystroke == PADENTER || (char)keystroke == '\n' || (char)keystroke == '\r'))
        {
            continue;
        }

        // user hit 'enter'
        input.clear();
        readStringFromWindow(userWindow, 1, 2, true, input);

        INFO_LOG("USER input str: ", input);
        if (!input.compare("quit") || !input.compare("exit"))
        {
            break;
        }
        std::vector<std::string> info;
        ActionStatus rc = cmdHandler.act(input, info);
        if (rc != ActionStatus::INFO)
        {
            // clear user window, reset cursor position
            mvwaddch(userWindow, 1, 1, '>');
            wclrtoeol(userWindow);
        }
        gameMap.printMap(gameWindow);
        input.clear();
        printBorder(gameWindow, COLOR_PAIR(ColorIndex::GAME_WIN_BORDER));
        printBorder(userWindow, COLOR_PAIR(ColorIndex::USER_WIN_BORDER));

        printToConsole(printoutPanel, info, false, 0);  // for some reason, printTo printout panel has to come after printBorder userWindow
        wrefresh(gameWindow);
        wrefresh(userWindow);
        update_panels();
        doupdate();
    }

    endwin();

    return 0;
}

void printToConsole(PANEL* const aPanel, const std::vector<std::string>& aMsg, bool aIsList, size_t aNormalSize)
{
    if (!aPanel)
    {
        ERROR_LOG("Nullptr: aPanel");
    }
    wclear(aPanel->win);
    int curY = 0;
    for (const std::string& msg : aMsg)
    {
        wattrset(aPanel->win, A_NORMAL);
        if (aIsList)
        {
            mvwaddch(aPanel->win, curY, 0, '|');
            mvwaddnstr(aPanel->win, curY + 1, 0, "|-", 2);
        }
        else
        {
            mvwaddnstr(aPanel->win, curY, 0, ">>", 2);
        }
        if (aNormalSize == 0)
        {
            waddnstr(aPanel->win, msg.c_str(), msg.length());
        }
        else
        {
            waddnstr(aPanel->win, msg.c_str(), aNormalSize);
            wattrset(aPanel->win, A_BOLD);
            waddnstr(aPanel->win, msg.c_str() + aNormalSize, msg.length() - aNormalSize);
        }
        curY += aIsList ? 2 : 1;
    }
    if (aMsg.size())
    {
        top_panel(aPanel);
    }
    else
    {
        hide_panel(aPanel);
    }
    update_panels();
    doupdate();
}

// aUntilEol: true - read until eol(two spaces); false - read up to cursor
int readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, std::string& aString)
{
    int curX, curY; // save cursor position, restore it later
    getyx(aWindow, curY, curX);
    char currPos = static_cast<char>(mvwinch(aWindow, aStartingY, aStartingX));
    if (aUntilEol)
    {
        char nextPos = static_cast<char>(mvwinch(aWindow, aStartingY, aStartingX + 1));
        while (currPos != ' ' || nextPos != ' ')
        {
            aString += currPos;
            ++aStartingX;
            currPos = static_cast<char>(mvwinch(aWindow, aStartingY, aStartingX));
            nextPos = static_cast<char>(mvwinch(aWindow, aStartingY, aStartingX + 1));
        }
    }
    else
    {
        while (aStartingX < curX)
        {
            aString += currPos;
            ++aStartingX;
            currPos = static_cast<char>(mvwinch(aWindow, aStartingY, aStartingX));
        }
    }
    wmove(aWindow, curY, curX);
    return aStartingX;
}

void printBorder(WINDOW* const aWindow, const chtype aColor)
{
    wborder(aWindow, aColor, aColor, aColor, aColor, aColor, aColor, aColor, aColor);
}

void checkSize(const GameMap& aMap, const int aBottomPadding, const int aRightPadding)
{
    int windowSizeVertical = aMap.getSizeVertical() + aBottomPadding;
    int windowSizeHorizontal = aMap.getSizeHorizontal() + aRightPadding;
    if (COLS < windowSizeHorizontal || LINES < windowSizeVertical)
    {
        ERROR_LOG("This game requires a window at least ", windowSizeVertical, "x", windowSizeHorizontal, " to run");
        endwin();
    }
}

// used when resize of map is out of order
void refreshAll(const GameMap& aMap, WINDOW* const aUserWindow, WINDOW* const aGameWindow, PANEL* const aPrintPanel)
{
    checkSize(aMap, 10, 10);
    resize_window(aUserWindow, LINES - aMap.getSizeVertical() - 1, COLS);
    mvwin(aUserWindow, aMap.getSizeVertical() + 2, 0);
    resize_window(aGameWindow, aMap.getSizeVertical(), aMap.getSizeHorizontal());

    wbkgd(aGameWindow, COLOR_PAIR(ColorIndex::GAME_WIN_BACKGROUND));
    wbkgd(aUserWindow, COLOR_PAIR(ColorIndex::USER_WIN_BACKGROUND));

    mvwin(aGameWindow, 0, 0);
    move_panel(aPrintPanel, aMap.getSizeVertical() + 3, 1);
    hide_panel(aPrintPanel);

    mvwaddch(aUserWindow, 1, 1, '>');
    printBorder(aGameWindow, COLOR_PAIR(ColorIndex::GAME_WIN_BORDER));
    printBorder(aUserWindow, COLOR_PAIR(ColorIndex::USER_WIN_BORDER));
    wrefresh(aUserWindow);
    wrefresh(aGameWindow);
    update_panels();
    doupdate();
}
