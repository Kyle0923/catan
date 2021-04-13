/**
 * Project: catan
 * @file user_interface.hpp
 * @brief Integration PDCurses library, handles CLI I/O
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_USER_INTERFACE_HPP
#define INCLUDE_USER_INTERFACE_HPP

#include <curses.h>
#include <panel.h>
#include "game_map.hpp"
#include "cli_command_manager.hpp"

enum ColorPairIndex
{
    COLOR_PAIR_INDEX_RESERVED = 0,  // 0 is reserved by PDCurses
    GAME_WIN,
    USER_WIN,
    PRINTOUT_WIN,
    GAME_WIN_BORDER,
    USER_WIN_BORDER
};

enum ColorIndex
{
    // 0 - 8 is used by PDCurses
    COLOR_INDEX_RESERVED = 9,
    COLOR_GREY
};

class UserInterface
{
private:
    WINDOW* mGameWindow;
    PANEL* mGamePanel;

    WINDOW* mInputWindow;
    PANEL* mInputPanel;

    WINDOW* mOutputWindow;
    PANEL* mOutputPanel;

    // coord of '>' in the input window
    const int inputStartX = 1;
    const int inputStartY = 1;

    int init(const GameMap& aMap);
    int printBorder(WINDOW* const aWindow, const ColorPairIndex aIndex);
    void restoreBorder(WINDOW* const aWindow, const chtype aColor);
    void resizeAll(const GameMap& aMap);

    // aUntilEol: true - read until end-of-line; false - read up to cursor
    int readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, std::string& aString);


    void printToConsole(const std::vector<std::string>& aMsg, bool aIsList, size_t aNormalSize);

public:
    bool checkSize(const GameMap& aMap, const int aVerticalPadding, \
                    const int aHorizontalPadding, const bool aExitOnFailure = false) const;

    int loop(GameMap& aMap, CliCommandManager& aCmdHandler);
    int printMapToWindow(const GameMap& aMap);
    UserInterface(const GameMap& aMap);
    ~UserInterface();
};

#endif /* INCLUDE_USER_INTERFACE_HPP */