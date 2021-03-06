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

#include <memory>
#include "command_helper.hpp"
#include "command_common.hpp"

/**
 * CommandParameterReader does not take ownership of mCmd,
 * i.e., it does not provide memory handling for mCmd.
 *
 * If memory handling is required for the command,
 * use FosterCommandParameterReader instead
*/
class CommandParameterReader : public CommandHelper
{
protected:
    CommandHandler* const mCmd;
public:
    CommandParameterReader(CommandHandler* const aCmd);
    virtual ~CommandParameterReader() = default;

    virtual std::vector<std::string> getPossibleInputs(const std::string& aInput, std::string* const aAutoFillString = nullptr) const override;

    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::string aInput, \
                               Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
};

class FosterCommandParameterReader : public CommandParameterReader
{
private:
    std::unique_ptr<CommandHandler> mUniquePtr;
public:
    explicit FosterCommandParameterReader(std::unique_ptr<CommandHandler> aUniquePtr);
    virtual ~FosterCommandParameterReader() = default;
};

#endif /* INCLUDE_COMMAND_PARAMETER_READER_HPP */
