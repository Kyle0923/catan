/**
 * Project: catan
 * @file common.hpp
 * @brief common data types
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_HISTORY_HPP
#define INCLUDE_COMMAND_HISTORY_HPP

#include<vector>
#include<string>

class CommandHistory{
private:
    std::vector<std::string>mHistory;
    size_t mCurrentIndex = 0;
    std::string mCurrentCommand;
public:
    CommandHistory();
    void recordCmd(std::string cmd);
    std::string getCommand();
    void nextCmd();
    void lastCmd();
};
#endif
