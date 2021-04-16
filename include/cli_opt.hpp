/**
 * Project: catan
 * @file cli_opt.hpp
 * @brief process command line parameters
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_CLI_OPT_HPP
#define INCLUDE_CLI_OPT_HPP

#include <tuple>
#include <array>
#include <cstring>
#include <cstddef>
#include "logger.hpp"
#include "constant.hpp"

/**
 * @usage:
 *  1. add the option to enum CliOptIndex so that it can be indexed
 *  2. add the type (int, std::string, etc..) of the option to 'using CliOptList_t = std::tuple<int>;'
 *  3. in CliOpt(), provide the matching string and the default value of the option, e.g,
 * '''
 *      cliOptNames.at(CliOptIndex::DEBUG_LEVEL) = "--debug";
 *      getOpt<CliOptIndex::DEBUG_LEVEL>() = 1;
 * '''
 *  4. add a case statement and use the extract{TYPE}Value<CliOptIndex> utility to extract values
 *     for Boolean flags, if you do not care about the value user passes in,
 *     you can directly set it to true (see 'case CliOptIndex::HELP_MANUAL')
 * '''
 *      case CliOptIndex::DEBUG_LEVEL:
 *          extractIntValue<CliOptIndex::DEBUG_LEVEL>(argc, argv, ii);
 *          break;
 *      case CliOptIndex::HELP_MANUAL:
 *          getOpt<CliOptIndex::HELP_MANUAL>() = true;
 *          break;
 * '''
 */

enum CliOptIndex
{
    DEBUG_LEVEL = 0,
    HELP_MANUAL,
    MAP_FILE_PATH,
    CLI_OPT_INDEX_END,
    /* end of CliOptIndex */
};

class CliOpt {

public:

    using CliOptList_t = std::tuple<int, bool, std::string>;

    template<CliOptIndex Index>
    using TYPE = typename std::tuple_element<Index, CliOptList_t >::type;

    CliOpt() {
        // setup the opt matching string and the default values
        cliOptNames.at(CliOptIndex::DEBUG_LEVEL) = "--debug";
        getOpt<CliOptIndex::DEBUG_LEVEL>() = constant::DEFAULT_DEBUG_LEVEL;
        cliOptNames.at(CliOptIndex::HELP_MANUAL) = "--help";
        getOpt<CliOptIndex::HELP_MANUAL>() = false;
        cliOptNames.at(CliOptIndex::MAP_FILE_PATH) = "--map";
        getOpt<CliOptIndex::MAP_FILE_PATH>() = "";
    }

    int processArg(const int argc, const char* const *argv)
    {
        for (int ii = 0; ii < argc; ++ii)
        {
            for (size_t optIndex = 0U; optIndex < CliOptIndex::CLI_OPT_INDEX_END; ++optIndex)
            {
                if ((cliOptNames.at(optIndex).find(argv[ii], 0, cliOptNames.at(optIndex).length()) == 0) && \
                    (argv[ii][cliOptNames.at(optIndex).length()] == '\0' || argv[ii][cliOptNames.at(optIndex).length()] == '='))
                {
                    // extract{TYPE}Value utility to extra values from argv
                    // e.g., extractIntValue<CliOptIndex::DEBUG_LEVEL>(argc, argv, ii)
                    switch (optIndex)
                    {
                    case CliOptIndex::DEBUG_LEVEL:
                        extractIntValue<CliOptIndex::DEBUG_LEVEL>(argc, argv, ii);
                        break;
                    case CliOptIndex::HELP_MANUAL:
                        getOpt<CliOptIndex::HELP_MANUAL>() = true;
                        break;
                    case CliOptIndex::MAP_FILE_PATH:
                        extractStringValue<CliOptIndex::MAP_FILE_PATH>(argc, argv, ii);
                        break;
                    default:
                        ERROR_LOG("Unhandled cli option: " + cliOptNames.at(optIndex) + ", index: ", optIndex);
                        break;
                    }
                    break;
                }
            }
        }
        return 0;
    }

    template< CliOptIndex Index >
    TYPE<Index>& getOpt()
    {
        return std::get<Index>(cliOptList);
    }

private:
    CliOptList_t cliOptList;
    std::array<std::string, std::tuple_size< CliOptList_t >::value> cliOptNames;

    template<CliOptIndex optIndex,
    typename std::enable_if<std::is_integral<TYPE<optIndex>>::value, void>::type* = nullptr >
    int extractIntValue(const int argc, const char* const *argv, int& aIndex)
    {
        return extracValue<optIndex>(argc, argv, aIndex, \
            [](const char* str, const char** endptr) {
                return static_cast< TYPE<optIndex> >(std::strtol(str, const_cast<char**>(endptr), 10));
            });
    }

    template<CliOptIndex optIndex,
    typename std::enable_if<std::is_floating_point<TYPE<optIndex>>::value, void>::type* = nullptr >
    int extracFloatValue(const int argc, const char* const *argv, int& aIndex)
    {
        return extracValue<optIndex>(argc, argv, aIndex, \
            [](const char* str, const char** endptr) {
                return static_cast< TYPE<optIndex> >(std::strtod(str, const_cast<char**>(endptr)));
            });
    }

    template<CliOptIndex optIndex,
    typename std::enable_if<std::is_same<TYPE<optIndex>, typename std::string>::value, void>::type* = nullptr >
    int extractStringValue(const int argc, const char* const *argv, int& aIndex)
    {
        return extracValue<optIndex>(argc, argv, aIndex, \
            [](const char* str, const char** endptr) {
                // mimic the behaviour of strtol and strtod
                std::string rtn(str);
                if (rtn == "") {
                    WARN_LOG("Empty string");
                    *endptr = str;
                } else {
                    *endptr = str + rtn.length();
                }
                return rtn;
            });
    }

    template<CliOptIndex optIndex,
    typename std::enable_if<std::is_same<TYPE<optIndex>, bool>::value, void>::type* = nullptr >
    int extractBooleanValue(const int argc, const char* const *argv, int& aIndex)
    {
        int rc = extractIntValue<optIndex>(argc, argv, aIndex);
        if (rc == 1)
        {
            // flag is set, but no value is provided
            INFO_LOG("Setting boolean flag '" + cliOptNames.at(optIndex) + "' to true");
            getOpt<optIndex>() = true;
        }
        return 0;
    }

    template<CliOptIndex optIndex>
    int extracValue(const int argc, const char* const *argv, int& aIndex, TYPE<optIndex> (*aStrToValue)(const char* str, const char** endptr))
    {
        int rc = 0;
        int incrementIndexOnSuccess = 0;
        const char* value = nullptr;
        if (std::strlen(argv[aIndex]) > cliOptNames.at(optIndex).length())
        {
            // passed in an option with equal sign, e.g., "--debug=1" (no space before and after "=" !!!)
            value = &(argv[aIndex][cliOptNames.at(optIndex).length() + 1]);
            INFO_LOG("Processing arg '", argv[aIndex], "', string value='", value, '\'');
            if (value[0] == '\0')
            {
                // if passed in "--debug=", value points at '\0'
                ERROR_LOG("Error processing arg '", argv[aIndex], "', no value was provided");
                return -1;
            }
        }
        else if ((std::strlen(argv[aIndex]) == cliOptNames.at(optIndex).length()) && (aIndex + 1 < argc))
        {
            // we have a string after current index
            value = argv[aIndex + 1]; // const ref to const char* of the value string
            INFO_LOG("Processing arg '" + cliOptNames.at(optIndex) + "', string value='" + value + '\'');
            incrementIndexOnSuccess = 1;
        }
        else
        {
            WARN_LOG("Discarded arg '" + cliOptNames.at(optIndex) + "', process reached EOL, no value was read (Booleans will be further processed)");
            return 1;
        }

        const char* endptr = value;  // avoid dangling pointer, no special meaning of the assigned value
        const TYPE<optIndex> typedValue = static_cast< TYPE<optIndex> >(aStrToValue(value, &endptr));
        if (!(errno == ERANGE || value == endptr))
        {
            INFO_LOG("Successfully read in arg '" + cliOptNames.at(optIndex) + "' value=" + value);
            getOpt<optIndex>() = typedValue;
            aIndex += incrementIndexOnSuccess;
        }
        else
        {
            WARN_LOG("Failed to read value, fallback to use default value, (Booleans will be further processed)");
            rc = 1;
        }
        return rc;
    }

    // check if index and opt size match
    static_assert(
        std::tuple_size< CliOptList_t >::value == CLI_OPT_INDEX_END,
        "CliOptList_t size and CliOptIndex::CLI_OPT_INDEX_END does not match, either missing index or missing type definition in CliOptList_t"
    );

}; // class CliOpt

#endif /* INCLUDE_CLI_OPT_HPP */
