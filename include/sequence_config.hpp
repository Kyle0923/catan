/**
 * Project: catan
 * @file sequence_config.hpp
 * @brief Used to randomize sequence
 *        SequenceConfig_t is an array whose value represents
 *        the num of occurrences of the index in the output
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_SEQUENCE_CONFIG_HPP
#define INCLUDE_SEQUENCE_CONFIG_HPP

#include <vector>
#include "common.hpp"

/* SequenceConfig_t is an array whose value represents the num of occurrences of the index in the output
 * e.g., aConfig[CLAY] = 2 means, there should be 2 CLAY in the returned vector
 */

struct SequenceConfig_t
{
    std::vector<size_t> mConfig;
    SequenceConfig_t(const size_t aSize);
    size_t size() const;
    size_t& operator[](const size_t aIndex);
    size_t& operator[](const ResourceTypes aIndex);
    size_t sum() const;
};

#endif /* INCLUDE_SEQUENCE_CONFIG_HPP */
