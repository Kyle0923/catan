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

ActionStatus StatelessCommandHandler::run(
    GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, \
    Point_t /* aPoint */, std::vector<std::string>& aReturnMsg)
{
    return statelessRun(aMap, aUi, aArgs, aReturnMsg);
}

ActionStatus StatefulCommandHandler::run(
    GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, \
    Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    // aArgs and aPoint are mutually exclusive
    // when aPoint != {0, 0} (a mouse event), then aArgs must contains no parameter (it should have an empty string)
    if (aPoint != Point_t{0, 0})
    {
        ActionStatus rc = onParameterReceive(aMap, "", aPoint, aReturnMsg);
        INFO_LOG(command() + "::onParameterReceive(), mouse_event: ", \
                    aPoint, "; returned " + actionStatusToStr(rc));
    }
    else
    {
        onStringParametersReceive(aMap, aArgs, aReturnMsg);
    }

    if (!parameterComplete())
    {
        instruction(aReturnMsg);
        return ActionStatus::PARAM_REQUIRED;
    }
    INFO_LOG("read param completed for " + command() + ", calling statefulRun()");

    ActionStatus rc = statefulRun(aMap, aUi, aReturnMsg);
    if (rc != ActionStatus::PARAM_REQUIRED)
    {
        resetParameters();
    }
    return rc;
}

ActionStatus StatefulCommandHandler::onStringParametersReceive(
    GameMap& aMap, const std::vector<std::string>& aArgs, \
    std::vector<std::string>& aReturnMsg)
{
    ActionStatus rc = ActionStatus::SUCCESS;
    for (const std::string& param : aArgs)
    {
        rc = onParameterReceive(aMap, param, Point_t{0, 0}, aReturnMsg);
        INFO_LOG(command() + "::onParameterReceive() param: " , param, "; returned " + actionStatusToStr(rc));

        if (rc != ActionStatus::SUCCESS)
        {
            aReturnMsg.emplace_back("Unkown parameter: " + param + ", abort");
            break;
        }
    }
    return rc;
}
