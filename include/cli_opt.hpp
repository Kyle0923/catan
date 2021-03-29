/**
 * Project: catan
 * @file cli_opt.hpp
 * @brief process command line inputs
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_CLI_OPT_HPP
#define INCLUDE_CLI_OPT_HPP

#include <tuple>
#include <array>
#include <cstddef>

/**
 * @usage:
 *  1. add the option to enum CliOptIndex so that it can be indexed
 *  2. add the type (int, std::string, etc..) of the option to 'using CliOptList_t = std::tuple<int>;'
 *  3. in CliOpt(), provide the matching string and the default value of the option, e.g,
 * '''
 *      cliOptNames.at(CliOptIndex::DEBUG_LEVEL) = "--debug";
 *      std::get<CliOptIndex::DEBUG_LEVEL>(cliOptList) = 1;
 * '''
 *  4. add a case statement and provide handling in processArg switch-case statement
 *     use std::get<CliOptIndex::$enum_value$>(cliOptList) to get a reference of the variable
 * '''
 *      case CliOptIndex::DEBUG_LEVEL:
 *          std::get<CliOptIndex::DEBUG_LEVEL>(cliOptList) = \
 *                  static_cast<int>(strtol(value, nullptr, 10));
 *          break;
 * '''
 */

enum CliOptIndex
{
    DEBUG_LEVEL = 0,
    CLI_OPT_INDEX_END,
};

class CliOpt {

public:

    using CliOptList_t = std::tuple<int>;

    CliOpt() {
        // setup the opt matching string and the default values
        cliOptNames.at(CliOptIndex::DEBUG_LEVEL) = "--debug";
        std::get<CliOptIndex::DEBUG_LEVEL>(cliOptList) = 1;
    }

    int processArg(const int argc, const char* const *argv)
    {
        for (int ii = 0; ii < argc; ++ii)
        {
            for (size_t index = 0U; index < CliOptIndex::CLI_OPT_INDEX_END; ++index)
            {
                if (cliOptNames.at(index).compare(argv[ii]) == 0)
                {
                    if (++ii < argc) {
                        const char* const& value = argv[ii]; // const ref to const char* of the value string
                        INFO_LOG("Processing arg '" + cliOptNames.at(index) + "' value='" + value + "'");
                        switch (index)
                        {
                        case CliOptIndex::DEBUG_LEVEL:
                            std::get<CliOptIndex::DEBUG_LEVEL>(cliOptList) = \
                                    static_cast<int>(strtol(value, nullptr, 10));
                            break;
                        default:
                            break;
                        }
                    }
                    else
                    {
                        WARN_LOG("Discarded arg '" + cliOptNames.at(index) + "' , process reached eol, no value was read");
                    }
                    break;
                }
            }
        }
        return 0;
    }

    template< CliOptIndex Index >
    const typename std::tuple_element<Index, CliOptList_t >::type&
    getOpt()
    {
        return std::get<Index>(cliOptList);
    }

private:
    CliOptList_t cliOptList;
    std::array<std::string, std::tuple_size< CliOptList_t >::value> cliOptNames;

    // check if index and opt size match
    static_assert(
        std::tuple_size< CliOptList_t >::value == CLI_OPT_INDEX_END,
        "CliOptList_t size and CliOptIndex::CLI_OPT_INDEX_END does not match, either missing index or missing type definition in CliOptList_t"
    );

}; // class CliOpt

#endif /* INCLUDE_CLI_OPT_HPP */
