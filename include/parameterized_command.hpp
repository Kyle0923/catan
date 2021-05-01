/**
 * Project: catan
 * @file parameterized_command.hpp
 * @brief ParameterizedCommand base class (interface)
 *        commands that requires parameter should inherit this class
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_PARAMETERIZED_COMMAND_HPP
#define INCLUDE_PARAMETERIZED_COMMAND_HPP

#include <vector>
#include "common.hpp"

class ParameterizedCommand
{
protected:
    std::vector<std::string> mParameters;
public:
    ParameterizedCommand();
    virtual ActionStatus readParameter(std::string);
    virtual ~ParameterizedCommand();
};

ParameterizedCommand::ParameterizedCommand(/* args */)
{
}

ParameterizedCommand::~ParameterizedCommand()
{
}

#endif /* INCLUDE_PARAMETERIZED_COMMAND_HPP */