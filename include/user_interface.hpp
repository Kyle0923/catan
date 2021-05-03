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

#include <memory>
#include <vector>
#include <curses.h>
#include <panel.h>
#include "command_helper.hpp"

class GameMap;

enum ColorPairIndex
{
    COLOR_PAIR_INDEX_RESERVED = 0,  // 0 is reserved by PDCurses
    GAME_WIN,
    USER_WIN,
    PRINTOUT_WIN,
    GAME_WIN_BORDER,
    USER_WIN_BORDER,
    PLAYER_START,
    PLAYER_END = PLAYER_START + 6,
#ifndef RELEASE
    TEMP    // for experiment purpose
#endif
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

    std::vector< std::unique_ptr<CommandHelper> > mCommandHelperStack;

    // coord of '>' in the input window
    int mInputStartX;
    int mInputStartY;

    int init(const GameMap& aMap, std::unique_ptr<CommandHelper>& aCmdDispatcher);
    int initColors();
    int printBorder(WINDOW* const aWindow, const ColorPairIndex aIndex);
    void restoreBorder(WINDOW* const aWindow, const chtype aColor);
    void resizeAll(const GameMap& aMap);
    void resizeOutputWindow();

    std::unique_ptr<CommandHelper>& currentCommandHelper();

    // aUntilEol: true - read until end-of-line; false - read up to cursor
    int readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, std::string& aString);

    /**
     * @param aIsList: print the vector using list format
     * @param aNormalSize: print up to aNormalSize in normal style, the rest are printed and grey out
     */
    void printToConsole(const std::vector<std::string>& aMsg, const std::string& aHeading, bool aIsList, size_t aNormalSize);
    void printToConsole(const std::string& aMsg);

public:
    static chtype getColorText(ColorPairIndex aColorIndex, char aCharacter);

    bool checkSize(const GameMap& aMap, const int aVerticalPadding, \
                    const int aHorizontalPadding, const bool aExitOnFailure = false) const;

    int pushCommandHelper(std::unique_ptr<CommandHelper> aCmdDispatcher);
    int loop(GameMap& aMap);
    int printMapToWindow(const GameMap& aMap);
    UserInterface(const GameMap& aMap, std::unique_ptr<CommandHelper> aCmdDispatcher);
    ~UserInterface();
};

#endif /* INCLUDE_USER_INTERFACE_HPP */
