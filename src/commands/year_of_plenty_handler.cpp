/**
 * Project: catan
 * @file year_of_plenty_handler.cpp
 *
 * @brief handle year_of_plenty development card
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_common.hpp"

const std::vector<std::string>& YearOfPlentyHandler::mResourceTypeMatchingPool = \
    consumableResourceStringValue;

YearOfPlentyHandler::YearOfPlentyHandler() :
    mResource1(ResourceTypes::NONE),
    mResource2(ResourceTypes::NONE)
{
    // empty
}

std::string YearOfPlentyHandler::command() const
{
    return "year_of_plenty_handler";
}

const std::vector<std::string>& YearOfPlentyHandler::paramAutoFillPool(size_t aParamIndex) const
{
    if (aParamIndex <= 1)
    {
        return mResourceTypeMatchingPool;
    }
    return mEmptyVector;
}

size_t YearOfPlentyHandler::currentParamIndex() const
{
    if (mResource1 == ResourceTypes::NONE)
    {
        return 0;
    }
    else if (mResource2 == ResourceTypes::NONE)
    {
        return 1;
    }
    return 2;
}

ActionStatus YearOfPlentyHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    for (std::string arg : aArgs)
    {
        onParameterReceive(aMap, arg, Point_t{0, 0}, aReturnMsg);
    }

    if (!parameterComplete())
    {
        return ActionStatus::PARAM_REQUIRED;
    }

    aMap.currentPlayerAddResource(mResource1);
    aMap.currentPlayerAddResource(mResource2);

    aReturnMsg.emplace_back (
            "You got 1 " + consumableResourceStringValue.at(static_cast<size_t>(mResource1)) + " and 1 "
                + consumableResourceStringValue.at(static_cast<size_t>(mResource2))
    );
    return ActionStatus::SUCCESS;
}

ActionStatus YearOfPlentyHandler::onParameterReceive(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (parameterComplete())
    {
        aReturnMsg.emplace_back("discarded parameter: " + aParam);
        return ActionStatus::SUCCESS;
    }

    ResourceTypes& resource = (mResource1 == ResourceTypes::NONE ? mResource1 : mResource2);
    int index = indexInVector(aParam, mResourceTypeMatchingPool);
    if (index == -1)
    {
        aReturnMsg.emplace_back("unkown parameter: " + aParam);
        return ActionStatus::FAILED;
    }
    resource = static_cast<ResourceTypes>(index);
    return ActionStatus::SUCCESS;
}

bool YearOfPlentyHandler::parameterComplete() const
{
    return (mResource1 != ResourceTypes::NONE && mResource2 != ResourceTypes::NONE);
}

void YearOfPlentyHandler::resetParameters()
{
    mResource1 = ResourceTypes::NONE;
    mResource2 = ResourceTypes::NONE;
}

void YearOfPlentyHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (mResource1 == ResourceTypes::NONE)
    {
        aReturnMsg.emplace_back("Which 2 resources you want to get?");
        aReturnMsg.emplace_back(stringVectorJoin(mResourceTypeMatchingPool));
        return;
    }
    if (mResource2 == ResourceTypes::NONE)
    {
        aReturnMsg.emplace_back("You chose " + mResourceTypeMatchingPool.at(static_cast<size_t>(mResource1)) + \
        ", what is the second resource you want?");
        aReturnMsg.emplace_back(stringVectorJoin(mResourceTypeMatchingPool));
        return;
    }
}
