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
#include <list>
#include <curses.h>
#include <panel.h>
#include "command_helper.hpp"

class GameMap;

enum ColorPairIndex
{
    COLOR_PAIR_INDEX_RESERVED = 0,  // 0 is reserved by PDCurses
    GAME_WIN,
    INPUT_WIN,
    OUTPUT_WIN,
    GAME_WIN_BORDER,
    PLAYER_START,
    PLAYER_END = PLAYER_START + 6,
};

enum ColorIndex
{
    // 0 - 8 is used by PDCurses
    COLOR_INDEX_RESERVED = 9,
    COLOR_GREY,
    COLOR_DARK_GREY,
    COLOR_DARK_GREEN,
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

    /**
     * @brief
     * the CommandHelperStack is implemented as a std::list as it requires frequent insertion / removal
     * with front being the most recently added element
     * i.e., pushCommandHelper() is a wrapper of emplace_front()
     *       and currentCommandHandler() is the front()
     */
    std::list< std::unique_ptr<CommandHelper> > mCommandHelperStack;

    // coord of '>' in the input window
    int mInputStartX;
    int mInputStartY;

    int init(const GameMap& aMap, std::unique_ptr<CommandHelper>& aCmdDispatcher);
    int initColors();
    int printBorder(WINDOW* const aWindow, const ColorPairIndex aIndex);
    ColorPairIndex getInputWinBorderColor(int aPlayerId);
    void restoreBorder(WINDOW* const aWindow, const chtype aColor);
    void resizeAll(const GameMap& aMap);
    void resizeOutputWindow();

    const CommandHelper* currentCommandHelper();

    /**
     * @param aUntilEol: true - read until end-of-line; false - read up to cursor
     * @param aString: the string object to hold the read-in value
     * @return the position of the last character
     */
    int readStringFromWindow(WINDOW* const aWindow, int aStartingY, int aStartingX, bool aUntilEol, bool aTrimLeadingSpace, std::string& aString);
    int readUserInput(bool aUntilEol, bool aTrimLeadingSpace, std::string& aString); // wrapper of readStringFromWindow

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
