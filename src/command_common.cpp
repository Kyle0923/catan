/**
 * Project: catan
 * @file command_common.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <map>
#include "command_handlers.hpp"
#include "logger.hpp"

const std::vector<std::string> CommandHandler::mEmptyVector = {};

std::string CommandHandler::description() const
{
    return "";
}

const std::vector<std::string>& CommandHandler::paramAutoFillPool(size_t aParamIndex) const
{
    return mEmptyVector;
}

size_t CommandHandler::currentParamIndex() const
{
    return 0;
}

ActionStatus StatelessCommandHandler::act(
    GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, \
    Point_t /* aPoint */, std::vector<std::string>& aReturnMsg)
{
    return actImpl(aMap, aUi, aArgs, aReturnMsg);
}

ActionStatus StatefulCommandHandler::act(
    GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, \
    Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    for (std::string param : aArgs)
    {
        ActionStatus rc;
        if (param == "")
        {
            rc = onParameterReceive(aMap, "", aPoint, aReturnMsg);
            INFO_LOG(command() + "::onParameterReceive(), mouse_event: ", \
                        aPoint, "; returned " + actionStatusToStr(rc));
        }
        else
        {
            rc = onParameterReceive(aMap, param, Point_t{0, 0}, aReturnMsg);
            INFO_LOG(command() + "::onParameterReceive(), mouse_event: ", \
                        aPoint, "; returned " + actionStatusToStr(rc));
        }

        if (rc != ActionStatus::SUCCESS)
        {
            break;
        }
    }
    if (!parameterComplete())
    {
        instruction(aReturnMsg);
        return ActionStatus::PARAM_REQUIRED;
    }
    INFO_LOG("read param completed for " + command() + ", calling actImpl()");

    ActionStatus rc = actImpl(aMap, aUi, aReturnMsg);
    if (rc != ActionStatus::PARAM_REQUIRED)
    {
        resetParameters();
    }
    return rc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

std::string ExitHandler::command() const
{
    return "exit";
}

std::string ExitHandler::description() const
{
    return "Exit current command";
}

ActionStatus ExitHandler::actImpl(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    return ActionStatus::EXIT;
}

std::string HelpHandler::command() const
{
    return "help";
}

std::string HelpHandler::description() const
{
    return "print help message";
}

ActionStatus HelpHandler::actImpl(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    const std::map<std::string, CommandHandler*>& handlers = mDispatcher->getHandlers();
    for (auto iter : handlers)
    {
        std::string cmd = iter.first;
        if (iter.second->description() != "")
        {
            cmd += " - " + iter.second->description();
        }
        if ( (aArgs.size() > 0 && indexInVector(iter.first, aArgs) >= 0)
             || (aArgs.size() == 0) )
        {
            aReturnMsg.push_back(cmd);
        }
    }
    return ActionStatus::SUCCESS;
}

HelpHandler::HelpHandler(CommandDispatcher* const aDispatcher): mDispatcher(aDispatcher)
{
    // empty
}

std::string NextHandler::command() const
{
    return "next";
}

std::string NextHandler::description() const
{
    return "Pass to next player";
}

ActionStatus NextHandler::actImpl(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    size_t nextPlayer = aMap.nextPlayer();
    aReturnMsg.push_back(Logger::formatString("Current Player is player#", nextPlayer));
    return ActionStatus::SUCCESS;
}

std::string PassHandler::command() const
{
    return "pass";
}