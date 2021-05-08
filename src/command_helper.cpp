/**
 * Project: catan
 * @file command_helper.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_helper.hpp"
#include "logger.hpp"

std::vector<std::string> CommandHelper::stringMatcher(const std::string& aInput, const std::vector<std::string>& aMatchPool, std::string* const aAutoFillString)
{
    // TODO: issue when there is whitespace after the last param, e.g, "he " returns "he lp"
    if (aMatchPool.size() == 1)
    {
        // matchPool size == 1, if aInput matches, return aMatchPool
        if (aMatchPool.front().find(aInput) == 0)
        {
            if (aAutoFillString)
            {
                *aAutoFillString = aMatchPool.front().substr(aInput.length()) + " ";
            }
            return aMatchPool;
        }
        else
        {
            return {};
        }
    }

    if (aInput.length() == 0 || aMatchPool.size() == 0)
    {
        // no input, return empty
        // prevent return everything in aMatchPool
        return {};
    }

    std::vector<std::string> matched;
    std::string shortestStr;
    for (const std::string& matchingString : aMatchPool)
    {
        bool found = false;
        if (matchingString.length() > aInput.size())
        {
            if (matchingString.find(aInput) == 0)
            {
                // cmd starts with aInput, partial match
                found = true;
            }
        }
        else
        {
            if (aInput.find(matchingString) == 0) // aInput includes matchingString
            {
                if (aInput.size() == matchingString.size() || aInput.at(matchingString.size()) == ' ')
                {
                    // either aInput is extra the same as matchingString or aInput has a whitespace terminating the command
                    // to prevent "helpX" matching "help"
                    found = true;
                }
            }
        }

        if (found)
        {
            matched.push_back(matchingString);
            if (matchingString.size() < shortestStr.size() || shortestStr.size() == 0)
            {
                shortestStr = matchingString;
            }
        }
    }
    DEBUG_LOG_L0("stringMatcher matched: ", matched);

    // if aAutoFillString is provided, return the auto complete part back via aAutoFillString
    if (aAutoFillString)
    {
        std::string longestCommonStr = aInput;
        if (matched.size() == 1 && matched.at(0).length() > aInput.size())
        {
            // full match, insert a whitespace at the end, "exi" => "exit "
            longestCommonStr = matched.at(0) + " ";
        }
        else if (matched.size() > 1)
        {
            // find the longest common string
            for (size_t ii = aInput.length(); ii < shortestStr.size(); ++ii)
            {
                char letter = matched.at(0).at(ii); // the ii-th char of the first matched string
                bool notmatch = false;
                for (size_t jj = 1; jj < matched.size(); ++jj)
                {
                    const std::string& str = matched.at(jj);
                    DEBUG_LOG_L0("cmp: ", str.at(ii), " & ", letter);
                    if (str.at(ii) != letter)
                    {
                        notmatch = true;
                        break;
                    }
                    if (jj == matched.size() - 1)
                    {
                        // all matched
                        longestCommonStr.push_back(letter);
                    }
                }
                if (notmatch)
                {
                    break;
                }
            }
        }
        *aAutoFillString = longestCommonStr.substr(aInput.length());
    }
    return matched;
}