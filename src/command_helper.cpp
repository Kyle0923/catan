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

std::vector<std::string> CommandHelper::stringMatcher(std::string aInput, const std::vector<std::string>& aMatchPool, std::string* const aLongestCommonStr = nullptr)
{
    // if (aInput.length() == 0)
    // {
    //     return {}; //return empty
    // }
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

    // if aLongestCommonStr is provided, return the longest common string back via aLongestCommonStr
    if (aLongestCommonStr)
    {
        *aLongestCommonStr = aInput;
        if (matched.size() == 1 && matched.at(0).length() > aInput.size())
        {
            *aLongestCommonStr = matched.at(0);
        }
        else if (matched.size() > 1)
        {
            std::string longestCommonSubstring;
            // find the longest common string
            for (size_t ii = aInput.length() - 1; ii < shortestStr.size(); ++ii)
            {
                char c = matched.at(0).at(ii); // the ii-th char of the first matched string
                bool notmatch = false;
                for (size_t jj = 1; jj < matched.size(); ++jj)
                {
                    const std::string& str = matched.at(jj);
                    DEBUG_LOG_L0("cmp: ", str.at(ii), " & ", c);
                    if (str.at(ii) != c)
                    {
                        notmatch = true;
                        break;
                    }
                    if (jj == matched.size() - 1)
                    {
                        // all matched
                        longestCommonSubstring = matched.at(0).substr(0, ii + 1);
                    }
                }
                if (notmatch)
                {
                    break;
                }
            }
            *aLongestCommonStr = longestCommonSubstring;
        }
    }
    return matched;
}