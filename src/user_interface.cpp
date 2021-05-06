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
    rc |= init_color(COLOR_DARK_GREY, 500, 500, 500);
    rc |= init_color(COLOR_DARK_GREEN, 50, 400, 50);

    rc |= init_pair(ColorPairIndex::GAME_WIN, COLOR_BLACK, COLOR_CYAN);
    rc |= init_pair(ColorPairIndex::INPUT_WIN, COLOR_BLACK, COLOR_WHITE);
    rc |= init_pair(ColorPairIndex::OUTPUT_WIN, COLOR_BLACK, COLOR_GREY);
    rc |= init_pair(ColorPairIndex::GAME_WIN_BORDER, COLOR_WHITE, COLOR_BLUE);

    // user color
    rc |= init_pair(ColorPairIndex::PLAYER_START + 0, COLOR_WHITE, COLOR_MAGENTA);
    rc |= init_pair(ColorPairIndex::PLAYER_START + 1, COLOR_BLACK, COLOR_YELLOW);
    rc |= init_pair(ColorPairIndex::PLAYER_START + 2, COLOR_WHITE, COLOR_RED);
    rc |= init_pair(ColorPairIndex::PLAYER_START + 3, COLOR_WHITE, COLOR_BLUE);
    rc |= init_pair(ColorPairIndex::PLAYER_START + 4, COLOR_WHITE, COLOR_DARK_GREEN);
    rc |= init_pair(ColorPairIndex::PLAYER_START + 5, COLOR_BLACK, COLOR_DARK_GREY);

    return rc;
}

int UserInterface::init(const GameMap& aMap, std::unique_ptr<CommandHelper>& aCmdDispatcher)
{
    mCommandHelperStack.clear();
    mCommandHelperStack.emplace_back(std::move(aCmdDispatcher));

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
    wbkgd(mInputWindow, COLOR_PAIR(ColorPairIndex::INPUT_WIN));
    wbkgd(mOutputWindow, COLOR_PAIR(ColorPairIndex::OUTPUT_WIN));

    printBorder(mGameWindow, ColorPairIndex::GAME_WIN_BORDER);
    printBorder(mInputWindow, static_cast<ColorPairIndex>(ColorPairIndex::PLAYER_START + static_cast<int>(aMap.currentPlayer())));

    mvwaddch(mInputWindow, mInputStartY, mInputStartX, '>');

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

ColorPairIndex UserInterface::getInputWinBorderColor(int aPlayerId)
{
    return static_cast<ColorPairIndex>(ColorPairIndex::PLAYER_START + aPlayerId);
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
    resizeOutputWindow();

    wbkgd(mGameWindow, COLOR_PAIR(ColorPairIndex::GAME_WIN));
    wbkgd(mInputWindow, COLOR_PAIR(ColorPairIndex::INPUT_WIN));
    wbkgd(mOutputWindow, COLOR_PAIR(ColorPairIndex::OUTPUT_WIN));

    wclear(mInputWindow);
    mvwaddch(mInputWindow, mInputStartY, mInputStartX, '>');
    printBorder(mGameWindow, ColorPairIndex::GAME_WIN_BORDER);
    printBorder(mInputWindow, getInputWinBorderColor(aMap.currentPlayer()));
    update_panels();
    doupdate();
}

void UserInterface::resizeOutputWindow()
{
    int inputWindowSizeX, inputWindowSizeY;
    getmaxyx(mInputWindow, inputWindowSizeY, inputWindowSizeX);
    resize_window(mOutputWindow, inputWindowSizeY - 2 - mInputStartY, inputWindowSizeX - 2);
    int inputWindowStartX, inputWindowStartY;
    getbegyx(mInputWindow, inputWindowStartY, inputWindowStartX);
    move_panel(mOutputPanel, inputWindowStartY + mInputStartY + 1, inputWindowStartX + 1);
}

int UserInterface::readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, bool aTrimLeadingSpace, std::string& aString)
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
    if (aTrimLeadingSpace)
    {
        trimLeadingSpace(aString);
    }
    wmove(aWindow, curY, curX);
    return aStartingX + aString.size();
}

void UserInterface::printToConsole(const std::vector<std::string>& aMsg, const std::string& aHeading, bool aIsList, size_t aNormalSize)
{
    wclear(mOutputWindow);
    resizeOutputWindow();

    int curY = 0;
    if (aHeading != "")
    {
        wattrset(mOutputWindow, A_BOLD);
        mvwaddstr(mOutputWindow, curY, 0, aHeading.c_str());
        ++curY;
    }

    for (const std::string& msg : aMsg)
    {
        wattrset(mOutputWindow, A_NORMAL);
        if (aIsList)
        {
            mvwaddch(mOutputWindow, curY, 0, '|');
            mvwaddstr(mOutputWindow, curY + 1, 0, "|-");
        }
        else
        {
            // mvwaddstr(mOutputWindow, curY, 0, "[system] ");
            wmove(mOutputWindow, curY, mInputStartX);
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
        curY = getcury(mOutputWindow) + 1;
    }
    if (aMsg.size() || aHeading != "")
    {
        DEBUG_LOG_L0("Printing heading '" + aHeading + "' and msg to panel: ", aMsg);
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
    printToConsole({aMsg}, "", false, 0);
}

int UserInterface::pushCommandHelper(std::unique_ptr<CommandHelper> aCmdDispatcher)
{
    mCommandHelperStack.emplace_back(std::move(aCmdDispatcher));
    ++mInputStartY;
    ++mInputStartX;
    return 0;
}

std::unique_ptr<CommandHelper>& UserInterface::currentCommandHelper()
{
    if (mCommandHelperStack.size() == 0)
    {
        ERROR_LOG("No command dispatcher available");
    }
    return mCommandHelperStack.back();
}

UserInterface::UserInterface(const GameMap& aMap, std::unique_ptr<CommandHelper> aCmdDispatcher) :
    mInputStartX(1),
    mInputStartY(1)
{
    init(aMap, aCmdDispatcher);
}

UserInterface::~UserInterface()
{
    endwin();
}

int UserInterface::loop(GameMap& aMap)
{
    // infinite loop preperation
    int spinCtr = 0;
    constexpr auto spinner = "/-\\|";
    std::string input = "";
    Point_t mouseEvent{0, 0};
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
                MEVENT clickEvent{0};
                if (ERR == nc_getmouse(&clickEvent))
                {
                    WARN_LOG("Encountered error when reading mouse event");
                }
                INFO_LOG("Mouse clicked, coord: [", clickEvent.x, ", ", clickEvent.y, "], key: ", clickEvent.bstate);
                if (!wenclose(mGameWindow, clickEvent.y, clickEvent.x))
                {
                    DEBUG_LOG_L1("Mouse event not in GameWindow, discard");
                    continue;
                }
                mouseEvent = Point_t{static_cast<size_t>(clickEvent.x), static_cast<size_t>(clickEvent.y)};
                break;
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
                wmove(mInputWindow, mInputStartY, mInputStartX + 1);
                wclrtoeol(mInputWindow);
                restoreBorder(mInputWindow, COLOR_PAIR(getInputWinBorderColor(aMap.currentPlayer())));
                break;
            }
            case KEY_BACKSPACE:
            case 8:   // ASCII 8 is backspace
            {
                int curX, curY;
                getyx(mInputWindow, curY, curX);
                if (curX > mInputStartX + 1)
                {
                    wmove(mInputWindow, curY, curX - 1);
                    wdelch(mInputWindow);
                    restoreBorder(mInputWindow, COLOR_PAIR(getInputWinBorderColor(aMap.currentPlayer())));
                }
                break;
            }
            case KEY_DC: //delete
            {
                wdelch(mInputWindow);
                restoreBorder(mInputWindow, COLOR_PAIR(getInputWinBorderColor(aMap.currentPlayer())));
                break;
            }
            case KEY_LEFT:
            {
                int curX, curY;
                getyx(mInputWindow, curY, curX);
                wmove(mInputWindow, curY, std::max(mInputStartX + 1, curX - 1));
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
                wmove(mInputWindow, mInputStartY, mInputStartX + 1);
                break;
            }
            case KEY_END:
            case KEY_NPAGE:
            {
                std::string str;
                int lastX = readStringFromWindow(mInputWindow, mInputStartY, mInputStartX + 1, true, false, str);
                wmove(mInputWindow, 1, lastX);
                break;
            }
            case '\t':
            {
                // auto fill
                readStringFromWindow(mInputWindow, mInputStartY, mInputStartX + 1, false, true, input);
                std::string autoFillString;
                std::vector<std::string> matched = currentCommandHelper()->getPossibleInputs(input, &autoFillString);
                if (autoFillString.length() > input.size())
                {
                    mvwaddstr(mInputWindow, mInputStartY, mInputStartX + 1, autoFillString.c_str());
                    wclrtoeol(mInputWindow);
                    restoreBorder(mInputWindow, COLOR_PAIR(getInputWinBorderColor(aMap.currentPlayer())));
                    wmove(mInputWindow, mInputStartY, mInputStartX + 1 + autoFillString.length());
                }
                break;
            }
        }

        if (isprint((char)keystroke) && keystroke <= 126)
        {
            // echo to mInputWindow, advance cursor
            winsch(mInputWindow, keystroke);
            restoreBorder(mInputWindow, COLOR_PAIR(getInputWinBorderColor(aMap.currentPlayer()))); // winsch will remove the border (char) at the end of current line
            int curX, curY;
            getyx(mInputWindow, curY, curX);
            wmove(mInputWindow, curY, curX + 1);
        }
        readStringFromWindow(mInputWindow, mInputStartY, mInputStartX + 1, false, true, input);
        // print matched commands to output window
        std::vector<std::string> matchedCmd = currentCommandHelper()->getPossibleInputs(input);
        printToConsole(matchedCmd, "", true, input.length());

        if (!(keystroke == KEY_ENTER || keystroke == PADENTER || keystroke == KEY_MOUSE \
            || (char)keystroke == '\n' || (char)keystroke == '\r'))
        {
            continue;
        }

        // user hit 'enter' || mouse event

        if (keystroke != KEY_MOUSE)
        {
            readStringFromWindow(mInputWindow, mInputStartY, mInputStartX + 1, true, true, input);
            INFO_LOG("USER input cmd: ", input);
        }
        else
        {
            input = "";
            INFO_LOG("USER input: mouse event");
        }
        const std::string inputPrefix(mInputStartX, '>');  // for later printToConsole

        std::vector<std::string> returnMsg;
        ActionStatus rc = currentCommandHelper()->act(aMap, *this, input, mouseEvent, returnMsg);
        if (rc == ActionStatus::EXIT)
        {
            mCommandHelperStack.pop_back();
            --mInputStartY;
            --mInputStartX;
        }

        if (mCommandHelperStack.size() == 0)
        {
            // no more handler, exit
            break;
        }

        if (rc != ActionStatus::PARTIAL_COMMAND)
        {
            // clear user window, reset cursor position
            wmove(mInputWindow, mInputStartY, 1);
            wclrtobot(mInputWindow);
            const std::string symbol(mInputStartX, '>');
            mvwaddstr(mInputWindow, mInputStartY, 1, symbol.c_str());
            wmove(mInputWindow, mInputStartY, mInputStartX + 1);
        }
        printToConsole(returnMsg, inputPrefix + input, false, 0);

        mouseEvent = Point_t{0, 0};
        printMapToWindow(aMap);
        printBorder(mGameWindow, ColorPairIndex::GAME_WIN_BORDER);
        printBorder(mInputWindow, getInputWinBorderColor(aMap.currentPlayer()));
        update_panels();
        doupdate();
    }
    return 0;
}