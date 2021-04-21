#include "sequence_config.hpp"

SequenceConfig_t::SequenceConfig_t(const size_t aSize) : mConfig(aSize, 0)
{
    //empty
}

size_t SequenceConfig_t::size() const
{
    return mConfig.size();
}

size_t SequenceConfig_t::sum() const
{
    size_t sum = 0;
    for (size_t num : mConfig)
    {
        sum += num;
    }
    return sum;
}

size_t& SequenceConfig_t::operator[](const size_t aIndex)
{
    return mConfig.at(aIndex);
}

size_t& SequenceConfig_t::operator[](const ResourceTypes aIndex)
{
    return mConfig.at(static_cast<size_t>(aIndex));
}
