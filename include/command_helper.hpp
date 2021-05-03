/**
 * Project: catan
 * @file command_helper.hpp
 * @brief base class of CommandDispatcher and CommandParameterReader
 *        to provide a common interface
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_HELPER_HPP
#define INCLUDE_COMMAND_HELPER_HPP

#include <vector>
#include "common.hpp"

class GameMap;
class UserInterface;

class CommandHelper
{
public:

    /**
     * find the strings in aMatchPool that starts with aInput
     * if aLongestCommonStr is provided, return the longest common part of the matched strings
     */
    static std::vector<std::string> \
        stringMatcher(std::string aInput, const std::vector<std::string>& aMatchPool, std::string* const aLongestCommonStr);

    /**
     * used for autoFill and print possible options to outputWindow
     */
    virtual std::vector<std::string> inputMatcher(std::string aInput, std::string* const aLongestCommonStr = nullptr) const = 0;

    virtual ActionStatus \
        act(GameMap& aMap, UserInterface& aUi, std::string aInput, Point_t aPoint, std::vector<std::string>& aReturnMsg) = 0;

    CommandHelper() = default;
    virtual ~CommandHelper() = default;
};

#endif /* INCLUDE_COMMAND_HELPER_HPP */
