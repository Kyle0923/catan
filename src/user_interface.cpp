/**
 * Project: catan
 * @file user_interface.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "user_interface.hpp"
#include "game_map.hpp"
#include "logger.hpp"
#include "utility.hpp"

int UserInterface::initColors()
{
    int rc = 0;
    rc |= init_color(COLOR_GREY, 700, 700, 700);

    rc |= init_pair(ColorPairIndex::GAME_WIN, COLOR_BLACK, COLOR_CYAN);
    rc |= init_pair(ColorPairIndex::USER_WIN, COLOR_BLACK, COLOR_WHITE);
    rc |= init_pair(ColorPairIndex::PRINTOUT_WIN, COLOR_BLACK, COLOR_GREY);
    rc |= init_pair(ColorPairIndex::GAME_WIN_BORDER, COLOR_WHITE, COLOR_BLUE);
    rc |= init_pair(ColorPairIndex::USER_WIN_BORDER, COLOR_WHITE, COLOR_YELLOW);
    rc |= init_pair(ColorPairIndex::TEMP, COLOR_GREEN, COLOR_YELLOW);
    return rc;
}

int UserInterface::init(const GameMap& aMap)
{
    initscr();
    resize_term(0, 0);

    checkSize(aMap, 10, 10, true);

    mGameWindow = newwin(aMap.getSizeVertical(), aMap.getSizeHorizontal(), 0, 0);
    mGamePanel = new_panel(mGameWindow);
    top_panel(mGamePanel);

    // user input panel
    mInputWindow = newwin(LINES - aMap.getSizeVertical() - 1, 0, aMap.getSizeVertical() + 1, 0);
    mInputPanel = new_panel(mInputWindow);
    bottom_panel(mInputPanel);

    // printout panel
    mOutputWindow = newwin(LINES - aMap.getSizeVertical() - 4, COLS - 2, aMap.getSizeVertical() + 3, 1);
    mOutputPanel = new_panel(mOutputWindow);
    bottom_panel(mOutputPanel);
    update_panels();

    keypad(mInputWindow, TRUE);  //return special keyboard stroke
    nocbreak();
    raw();      //capture ctrl-C etc...
    noecho();
    wtimeout(mInputWindow, 200);  //read input timeout 0.2sec

    //setup mouse
    mousemask(BUTTON1_CLICKED, nullptr);

    if (!has_colors())
    {
        ERROR_LOG("Console does not support color");
    }

    start_color();
    initColors();

    wbkgd(mGameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN));
    wbkgd(mInputWindow, COLOR_PAIR(ColorPairIndex::USER_WIN));
    wbkgd(mOutputWindow, COLOR_PAIR(ColorPairIndex::PRINTOUT_WIN));

    printBorder(mGameWindow, ColorPairIndex::GAME_WIN_BORDER);
    printBorder(mInputWindow, ColorPairIndex::USER_WIN_BORDER);

    mvwaddch(mInputWindow, inputStartY, inputStartX, '>');

    update_panels();
    doupdate();

    return 0;
}

int UserInterface::printMapToWindow(const GameMap& aMap)
{
    const std::deque< std::deque<Terrain*> >& map = aMap.getTerrainMap();
    for (size_t jj = 0; jj < map.size(); ++jj)
    {
        const std::deque<Terrain*>& row = map.at(jj);
        for (size_t ii = 0; ii < row.size(); ++ii)
        {
            // easier to debug using an extra char c
            chtype colorChar = row.at(ii)->getColorCharRepresentation(ii, jj);
            mvwaddch(mGameWindow, jj, ii, colorChar);
        }
    }
    printBorder(mGameWindow, GAME_WIN_BORDER);
    update_panels();
    doupdate();
    return 0;
}

chtype UserInterface::getColorText(ColorPairIndex aColorIndex, char aCharacter)
{
    if (aColorIndex == ColorPairIndex::COLOR_PAIR_INDEX_RESERVED)
    {
        return aCharacter;
    }
    else
    {
        return COLOR_PAIR(aColorIndex) | aCharacter;
    }
}

int UserInterface::printBorder(WINDOW* const aWindow, const ColorPairIndex aIndex)
{
    const chtype color = COLOR_PAIR(aIndex);
    return wborder(aWindow, color, color, color, color, color, color, color, color);
}

bool UserInterface::checkSize(const GameMap& aMap, const int aVerticalPadding, const int aHorizontalPadding, bool aExitOnFailure) const
{
    int windowSizeVertical = aMap.getSizeVertical() + aVerticalPadding;
    int windowSizeHorizontal = aMap.getSizeHorizontal() + aHorizontalPadding;
    if (COLS < windowSizeHorizontal || LINES < windowSizeVertical)
    {
        if (aExitOnFailure)
        {
            endwin();
            ERROR_LOG("This game requires a window at least ", windowSizeVertical, 'x', windowSizeHorizontal, " to run, " \
                        "current window size: ", LINES, 'x', COLS);
        }
        return false;
    }
    INFO_LOG("checkSize passed, COLS: ", COLS, " LINES: ", LINES);
    return true;
}


void UserInterface::restoreBorder(WINDOW* const aWindow, const chtype aColor)
{
    int curX, curY; // save cursor position, restore it later
    getyx(aWindow, curY, curX);
    const int maxX = getmaxx(aWindow) - 1; // max index = size - 1
    wmove(aWindow, curY, maxX - 1);
    wclrtoeol(aWindow);
    mvwaddch(aWindow, curY, maxX, aColor|' ');
    wmove(aWindow, curY, curX);
}

void UserInterface::resizeAll(const GameMap& aMap)
{
    if (!checkSize(aMap, 10, 10, false))
    {
        return;
    }
    resize_window(mInputWindow, LINES - aMap.getSizeVertical() - 1, COLS);
    resize_window(mGameWindow, aMap.getSizeVertical(), aMap.getSizeHorizontal());
    resize_window(mOutputWindow, LINES - aMap.getSizeVertical() - 4, COLS - 2);

    wbkgd(mGameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN));
    wbkgd(mInputWindow, COLOR_PAIR(ColorPairIndex::USER_WIN));
    wbkgd(mOutputWindow, COLOR_PAIR(ColorPairIndex::PRINTOUT_WIN));

    wclear(mInputWindow);
    mvwaddch(mInputWindow, 1, 1, '>');
    printBorder(mGameWindow, ColorPairIndex::GAME_WIN_BORDER);
    printBorder(mInputWindow, ColorPairIndex::USER_WIN_BORDER);
    update_panels();
    doupdate();
}

int UserInterface::readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, std::string& aString)
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
    trimTrailingSpace(aString);

    wmove(aWindow, curY, curX);
    return aStartingX + aString.size();
}

void UserInterface::printToConsole(const std::vector<std::string>& aMsg, bool aIsList, size_t aNormalSize)
{
    wclear(mOutputWindow);
    int curY = 0;
    for (const std::string& msg : aMsg)
    {
        wattrset(mOutputWindow, A_NORMAL);
        if (aIsList)
        {
            mvwaddch(mOutputWindow, curY, 0, '|');
            mvwaddnstr(mOutputWindow, curY + 1, 0, "|-", 2);
        }
        else
        {
            mvwaddnstr(mOutputWindow, curY, 0, ">>", 2);
        }
        if (aNormalSize == 0)
        {
            waddnstr(mOutputWindow, msg.c_str(), msg.length());
        }
        else
        {
            const size_t normalSize = std::min(msg.length(), aNormalSize);
            waddnstr(mOutputWindow, msg.c_str(), normalSize);
            wattrset(mOutputWindow, A_BOLD);
            waddnstr(mOutputWindow, msg.c_str() + normalSize, msg.length() - normalSize);
        }
        curY = getcury(mOutputWindow) + (aIsList ? 2 : 1);
    }
    if (aMsg.size())
    {
        DEBUG_LOG_L0("Printing msg to panel: ", aMsg);
        top_panel(mOutputPanel);
    }
    else
    {
        DEBUG_LOG_L0("hiding printout panel");
        hide_panel(mOutputPanel);
    }
    update_panels();
    doupdate();
}

void UserInterface::printToConsole(const std::string& aMsg)
{
    printToConsole({aMsg}, false, 0);
}

UserInterface::UserInterface(const GameMap& aMap)
{
    init(aMap);
}

UserInterface::~UserInterface()
{
    endwin();
}

int UserInterface::loop(GameMap& aMap, CliCommandManager& aCmdHandler)
{
    // infinite loop preperation
    int spinCtr = 0;
    constexpr auto spinner = "/-\\|";
    std::string input = "";
    int keystroke;

    while (1)
    {
        keystroke = wgetch(mInputWindow);
        if (keystroke == ERR)
        {
            mvwaddch(mGameWindow, 1, 1, spinner[++spinCtr % 4]);
            wrefresh(mGameWindow);
            continue;
        }

        DEBUG_LOG_L3("keystroke recorded: ", (char)keystroke, " int: ", keystroke);
        switch (keystroke)
        {
            case 27: // esc
            {
                return 0;
            }
            case KEY_MOUSE:
            {
                MEVENT mouseEvent{0};
                if (ERR == nc_getmouse(&mouseEvent))
                {
                    WARN_LOG("Encountered error when reading mouse event");
                }
                INFO_LOG("Mouse clicked, coord: [", mouseEvent.x, ", ", mouseEvent.y, "], key: ", mouseEvent.bstate);
                if (!wenclose(mGameWindow, mouseEvent.y, mouseEvent.x))
                {
                    DEBUG_LOG_L1("Mouse event not in GameWindow, discard");
                    continue;
                }
                const std::string id = aMap.getTerrain(mouseEvent.x, mouseEvent.y)->getStringId();
                aMap.setTerrainColor(mouseEvent.x, mouseEvent.y, ColorPairIndex::TEMP);
                printToConsole("Clicked " + id);
                continue;
            }
            case KEY_RESIZE:
            case KEY_F(5): // F5 function key
            {
                INFO_LOG("Performing resizing (resizing can occasionally crash the program)");
                resize_term(0, 0);
                if (!checkSize(aMap, 10, 10, false))
                {
                    resize_term(aMap.getSizeVertical() + 10, aMap.getSizeHorizontal() + 10);
                }
                resizeAll(aMap);
                continue;
            }
            case 3:  // ASCII 3 ctrl-C
            case 4:  // ASCII 4 ctrl-D
            {
                wmove(mInputWindow, inputStartY, inputStartX + 1);
                wclrtoeol(mInputWindow);
                restoreBorder(mInputWindow, COLOR_PAIR(USER_WIN_BORDER));
                break;
            }
            case KEY_BACKSPACE:
            case 8:   // ASCII 8 is backspace
            {
                int curX, curY;
                getyx(mInputWindow, curY, curX);
                if (curX > 2)
                {
                    wmove(mInputWindow, curY, curX - 1);
                    wdelch(mInputWindow);
                    restoreBorder(mInputWindow, COLOR_PAIR(USER_WIN_BORDER));
                }
                break;
            }
            case KEY_DC: //delete
            {
                wdelch(mInputWindow);
                restoreBorder(mInputWindow, COLOR_PAIR(USER_WIN_BORDER));
                break;
            }

            case KEY_LEFT:
            {
                int curX, curY;
                getyx(mInputWindow, curY, curX);
                wmove(mInputWindow, curY, std::max(inputStartX + 1, curX - 1));
                break;
            }
            case KEY_RIGHT:
            {
                int curX, curY;
                getyx(mInputWindow, curY, curX);
                char currPos = static_cast<char>(mvwinch(mInputWindow, curY, curX));
                char nextPos = static_cast<char>(mvwinch(mInputWindow, curY, curX + 1));
                if (nextPos == ' ' && currPos == ' ')
                {
                    DEBUG_LOG_L2("Key Right reached EOL");
                    // reached eol, put cursor back
                    wmove(mInputWindow, curY, curX);
                }
                break;
            }
            case KEY_HOME:
            case KEY_PPAGE:
            {
                wmove(mInputWindow, inputStartY, inputStartX + 1);
                break;
            }
            case KEY_END:
            case KEY_NPAGE:
            {
                std::string str;
                int lastX = readStringFromWindow(mInputWindow, inputStartY, inputStartX + 1, true, str);
                wmove(mInputWindow, 1, lastX);
                break;
            }
            case '\t':
            {
                // auto fill
                readStringFromWindow(mInputWindow, inputStartY, inputStartX + 1, false, input);
                std::string autoFillString;
                std::vector<std::string> matched = aCmdHandler.commandMatcher(input, &autoFillString);
                if (matched.size() == 1)
                {
                    // overwrite the longestCommonStr to the matched string
                    autoFillString = matched.at(0);
                }
                if (autoFillString.length() > input.size())
                {
                    mvwaddnstr(mInputWindow, inputStartY, inputStartX + 1, autoFillString.c_str(), autoFillString.length());
                    wmove(mInputWindow, inputStartY, 2 + autoFillString.length());
                }
                break;
            }
        }

        if (isprint((char)keystroke) && keystroke <= 126)
        {
            // echo to mInputWindow, advance cursor
            winsch(mInputWindow, keystroke);
            restoreBorder(mInputWindow, COLOR_PAIR(ColorPairIndex::USER_WIN_BORDER)); // winsch will remove the border (char) at the end of current line
            int curX, curY;
            getyx(mInputWindow, curY, curX);
            wmove(mInputWindow, curY, curX + 1);
        }
        readStringFromWindow(mInputWindow, inputStartY, inputStartX + 1, false, input);
        std::vector<std::string> matchedCmd = aCmdHandler.commandMatcher(input);
        printToConsole(matchedCmd, true, input.length());

        if (!(keystroke == KEY_ENTER || keystroke == PADENTER || (char)keystroke == '\n' || (char)keystroke == '\r'))
        {
            continue;
        }

        // user hit 'enter'
        readStringFromWindow(mInputWindow, inputStartY, inputStartX + 1, true, input);

        INFO_LOG("USER input cmd: ", input);
        if (!input.compare("quit") || !input.compare("exit"))
        {
            break;
        }
        std::vector<std::string> info;
        ActionStatus rc = aCmdHandler.act(input, info);
        if (rc != ActionStatus::INCOMPLETE)
        {
            // clear user window, reset cursor position
            wclear(mInputWindow);
            mvwaddch(mInputWindow, inputStartY, inputStartX, '>');
        }
        printToConsole(info, false, 0);

        printMapToWindow(aMap);
        printBorder(mGameWindow, ColorPairIndex::GAME_WIN_BORDER);
        printBorder(mInputWindow, ColorPairIndex::USER_WIN_BORDER);
        update_panels();
        doupdate();
    }
    return 0;
}