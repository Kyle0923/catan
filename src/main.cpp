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

enum ColorPairIndex
{
    COLOR_PAIR_INDEX_RESERVED = 0,  // 0 is reserved by PDCurses
    GAME_WIN_BACKGROUND,
    USER_WIN_BACKGROUND,
    PRINTOUT_WIN_BACKGROUND,
    GAME_WIN_BORDER,
    USER_WIN_BORDER
};

enum ColorIndex
{
    // 0 - 8 is used by PDCurses
    COLOR_INDEX_RESERVED = 9,
    COLOR_GREY
};

void printToConsole(PANEL* const aPanel, const std::vector<std::string>& aMsg, bool aIsList, size_t aNormalSize = 0);
int readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, std::string& aString);
void printBorder(WINDOW* const aWindow, const chtype aColor);
void restoreBorder(WINDOW* const aWindow, const chtype aColor); // used when pressing delete or backspace
bool checkSize(const GameMap& aMap, const int aBottomPadding, const int aRightPadding, const bool aExitOnFailure);
void resizeAll(const GameMap& aMap, WINDOW* const aUserWindow, WINDOW* const aGameWindow, PANEL* const aPrintPanel);

int main(int argc, char** argv)
{
    Logger::initLogfile();
    INFO_LOG("Begin of Catan.exe");

    CliOpt cliOpt;
    cliOpt.processArg(argc, argv);
    Logger::setDebugLevel(cliOpt.getOpt<CliOptIndex::DEBUG_LEVEL>());

    GameMap gameMap;
    MapIO mapFile("map.txt");
    mapFile.readMap(gameMap);

    initscr();
    resize_term(0, 0);

    checkSize(gameMap, 10, 10, true);

    WINDOW* gameWindow = newwin(gameMap.getSizeVertical(), gameMap.getSizeHorizontal(), 0, 0);
    PANEL* gamePanel = new_panel(gameWindow);
    top_panel(gamePanel);

    // user panel
    WINDOW* userWindow = newwin(LINES - gameMap.getSizeVertical() - 1, 0, gameMap.getSizeVertical() + 1, 0);
    PANEL* userPanel = new_panel(userWindow);
    bottom_panel(userPanel);

    // printout panel
    WINDOW* printoutWindow = newwin(LINES - gameMap.getSizeVertical() - 4, COLS - 2, gameMap.getSizeVertical() + 3, 1);
    PANEL* printoutPanel = new_panel(printoutWindow);
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

    init_color(COLOR_GREY, 700, 700, 700);

    init_pair(ColorPairIndex::GAME_WIN_BACKGROUND, COLOR_BLACK, COLOR_CYAN);
    init_pair(ColorPairIndex::USER_WIN_BACKGROUND, COLOR_BLACK, COLOR_WHITE);
    init_pair(ColorPairIndex::PRINTOUT_WIN_BACKGROUND, COLOR_BLACK, COLOR_GREY);
    init_pair(ColorPairIndex::GAME_WIN_BORDER, COLOR_WHITE, COLOR_BLUE);
    init_pair(ColorPairIndex::USER_WIN_BORDER, COLOR_WHITE, COLOR_YELLOW);

    wbkgd(gameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN_BACKGROUND));
    wbkgd(userWindow, COLOR_PAIR(ColorPairIndex::USER_WIN_BACKGROUND));
    wbkgd(printoutWindow, COLOR_PAIR(ColorPairIndex::PRINTOUT_WIN_BACKGROUND));

    gameMap.initMap();
    gameMap.printMap(gameWindow);

    printBorder(gameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN_BORDER));
    printBorder(userWindow, COLOR_PAIR(ColorPairIndex::USER_WIN_BORDER));

    // coord of '>' in the input window
    constexpr int inputX = 1;
    constexpr int inputY = 1;
    mvwaddch(userWindow, inputY, inputX, '>');

    update_panels();
    doupdate();

    CliCommandManager cmdHandler;
    cmdHandler.init();

    // infinite loop preperation
    int spinCtr = 0;
    std::string spinner = "/-\\|";
    std::string input = "";
    int keystroke;

    while (1)
    {
        keystroke = wgetch(userWindow);
        if (keystroke == ERR)
        {
            mvwaddch(gameWindow, 1, 1, spinner[++spinCtr % 4]);
            wrefresh(gameWindow);
            continue;
        }
        else if (keystroke == 27) // esc
        {
            break;
        }

        INFO_LOG("keystroke recorded: ", (char)keystroke, " int: ", keystroke);
        switch (keystroke)
        {
            case KEY_RESIZE:
            case KEY_F(5): // F5 function key
            {
                INFO_LOG("Performing resizing (resizing can occasionally crash the program)");
                if (!checkSize(gameMap, 10, 10, false))
                {
                    resize_term(gameMap.getSizeVertical() + 10, gameMap.getSizeHorizontal() + 10);
                    resizeAll(gameMap, userWindow, gameWindow, printoutPanel);
                    continue;
                }
                else
                {
                    resize_term(0, 0);
                    resizeAll(gameMap, userWindow, gameWindow, printoutPanel);
                }
                break;
            }
            case 3:  // ASCII 3 ctrl-C
            case 4:  // ASCII 4 ctrl-D
            {
                wmove(userWindow, inputY, inputX + 1);
                wclrtoeol(userWindow);
                restoreBorder(userWindow, COLOR_PAIR(USER_WIN_BORDER));
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
                    restoreBorder(userWindow, COLOR_PAIR(USER_WIN_BORDER));
                }
                break;
            }
            case KEY_DC: //delete
            {
                wdelch(userWindow);
                restoreBorder(userWindow, COLOR_PAIR(USER_WIN_BORDER));
                break;
            }

            case KEY_LEFT:
            {
                int curX, curY;
                getyx(userWindow, curY, curX);
                wmove(userWindow, curY, std::max(inputX + 1, curX - 1));
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
                wmove(userWindow, inputY, inputX + 1);
                break;
            }
            case KEY_END:
            case KEY_NPAGE:
            {
                std::string str;
                int lastX = readStringFromWindow(userWindow, inputY, inputX + 1, true, str);
                wmove(userWindow, 1, lastX);
                break;
            }
            case '\t':
            {
                // auto fill
                readStringFromWindow(userWindow, inputY, inputX + 1, false, input);
                std::string autoFillString;
                std::vector<std::string> matched = cmdHandler.commandMatcher(input, &autoFillString);
                if (matched.size() == 1)
                {
                    // overwrite the longestCommonStr to the matched string
                    autoFillString = matched.at(0);
                }
                if (autoFillString.length() > input.size())
                {
                    mvwaddnstr(userWindow, inputY, inputX + 1, autoFillString.c_str(), autoFillString.length());
                    wmove(userWindow, inputY, 2 + autoFillString.length());
                }
                break;
            }
        }

        if (isprint((char)keystroke) && keystroke <= 126)
        {
            // echo to userWindow, advance cursor
            winsch(userWindow, keystroke);
            restoreBorder(userWindow, COLOR_PAIR(ColorPairIndex::USER_WIN_BORDER)); // winsch will remove the border (char) at the end of current line
            int curX, curY;
            getyx(userWindow, curY, curX);
            wmove(userWindow, curY, curX + 1);
        }
        readStringFromWindow(userWindow, inputY, inputX + 1, false, input);
        std::vector<std::string> matchedCmd = cmdHandler.commandMatcher(input);
        printToConsole(printoutPanel, matchedCmd, true, input.length());

        if (!(keystroke == KEY_ENTER || keystroke == PADENTER || (char)keystroke == '\n' || (char)keystroke == '\r'))
        {
            continue;
        }

        // user hit 'enter'
        readStringFromWindow(userWindow, inputY, inputX + 1, true, input);

        INFO_LOG("USER input cmd: ", input);
        if (!input.compare("quit") || !input.compare("exit"))
        {
            break;
        }
        std::vector<std::string> info;
        ActionStatus rc = cmdHandler.act(input, info);
        if (rc != ActionStatus::INCOMPLETE)
        {
            // clear user window, reset cursor position
            wclear(userWindow);
            mvwaddch(userWindow, inputY, inputX, '>');
        }
        printToConsole(printoutPanel, info, false, 0);

        gameMap.printMap(gameWindow);
        printBorder(gameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN_BORDER));
        printBorder(userWindow, COLOR_PAIR(ColorPairIndex::USER_WIN_BORDER));
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
            const size_t normalSize = std::min(msg.length(), aNormalSize);
            waddnstr(aPanel->win, msg.c_str(), normalSize);
            wattrset(aPanel->win, A_BOLD);
            waddnstr(aPanel->win, msg.c_str() + normalSize, msg.length() - normalSize);
        }
        curY += aIsList ? 2 : 1;
    }
    if (aMsg.size())
    {
        DEBUG_LOG_L0("Printing msg to panel: ", aMsg);
        top_panel(aPanel);
    }
    else
    {
        DEBUG_LOG_L0("hiding printout panel");
        hide_panel(aPanel);
    }
    update_panels();
    doupdate();
}

// aUntilEol: true - read until end-of-line; false - read up to cursor
int readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, std::string& aString)
{
    int curX, curY; // save cursor position, restore it later
    getyx(aWindow, curY, curX);
    const int bufferSize = COLS - aStartingX;
    char buffer[bufferSize] = {0};
    if (aUntilEol)
    {
        mvwinnstr(aWindow, aStartingY, aStartingX, buffer, bufferSize);
    }
    else
    {
        if (curX - aStartingX + 1 <= 0)
        {
            WARN_LOG("curX - aStartingX + 1 <= 0");
        }
        else
        {
            mvwinnstr(aWindow, aStartingY, aStartingX, buffer, curX - aStartingX + 1);
        }
    }
    aString = std::string(buffer);
    // trim trailing space
    while (aString.back() == ' ')
    {
        aString.pop_back();
    }

    wmove(aWindow, curY, curX);
    return aStartingX + aString.size();
}

void printBorder(WINDOW* const aWindow, const chtype aColor)
{
    // const chtype border = aColor | '#';    // maybe? use # as delimiter
    // wborder(aWindow, border, border, border, border, border, border, border, border);
    wborder(aWindow, aColor, aColor, aColor, aColor, aColor, aColor, aColor, aColor);
}

void restoreBorder(WINDOW* const aWindow, const chtype aColor)
{
    int curX, curY; // save cursor position, restore it later
    getyx(aWindow, curY, curX);
    const int maxX = getmaxx(aWindow) - 1; // max index = size - 1
    wmove(aWindow, curY, maxX - 1);
    wclrtoeol(aWindow);
    mvwaddch(aWindow, curY, maxX, aColor|' ');
    wmove(aWindow, curY, curX);
}

bool checkSize(const GameMap& aMap, const int aBottomPadding, const int aRightPadding, const bool aExitOnFailure)
{
    int windowSizeVertical = aMap.getSizeVertical() + aBottomPadding;
    int windowSizeHorizontal = aMap.getSizeHorizontal() + aRightPadding;
    if (COLS < windowSizeHorizontal || LINES < windowSizeVertical)
    {
        if (aExitOnFailure)
        {
            endwin();
            ERROR_LOG("This game requires a window at least ", windowSizeVertical, "x", windowSizeHorizontal, " to run");
        }
        return false;
    }
    INFO_LOG("checkSize passed, COLS: ", COLS, " LINES: ", LINES);
    return true;
}

// used when resize of map is out of order
void resizeAll(const GameMap& aMap, WINDOW* const aUserWindow, WINDOW* const aGameWindow, PANEL* const aPrintPanel)
{
    if (!checkSize(aMap, 10, 10, false))
    {
        return;
    }
    resize_window(aUserWindow, LINES - aMap.getSizeVertical() - 1, COLS);
    resize_window(aGameWindow, aMap.getSizeVertical(), aMap.getSizeHorizontal());
    resize_window(aPrintPanel->win, LINES - aMap.getSizeVertical() - 4, COLS - 2);

    wbkgd(aGameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN_BACKGROUND));
    wbkgd(aUserWindow, COLOR_PAIR(ColorPairIndex::USER_WIN_BACKGROUND));
    wbkgd(aPrintPanel->win, COLOR_PAIR(ColorPairIndex::PRINTOUT_WIN_BACKGROUND));

    wclear(aUserWindow);
    mvwaddch(aUserWindow, 1, 1, '>');
    printBorder(aGameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN_BORDER));
    printBorder(aUserWindow, COLOR_PAIR(ColorPairIndex::USER_WIN_BORDER));
    update_panels();
    doupdate();
}
