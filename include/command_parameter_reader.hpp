/**
 * Project: catan
 * @file command_parameter_reader.hpp
 * @brief CommandParameterReader helps commands process their parameters
 *        If the command reports parameter-complete, it will call CommandHandler::act()
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_PARAMETER_READER_HPP
#define INCLUDE_COMMAND_PARAMETER_READER_HPP

#include "command_helper.hpp"
#include "command_handler.hpp"

class CommandParameterReader : public CommandHelper
{
private:
    CommandHandler* const mCmd;
public:
    CommandParameterReader(CommandHandler* const aCmd);
    virtual ~CommandParameterReader() = default;

    virtual std::vector<std::string> getPossibleInputs(const std::string& aInput, std::string* const aLongestCommonStr = nullptr) const override;

    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::string aInput, \
                               Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
};

#endif /* INCLUDE_COMMAND_PARAMETER_READER_HPP */
