/**
 * Project: catan
 * @file map_file_io.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "map_file_io.hpp"
#include "logger.hpp"
#include "utility.hpp"

int MapIO::readMap(GameMap& aGameMap)
{
    std::deque<std::string> stringQueue;
    if (mFilename == "" || mFilename == "default")
    {
        DEBUG_LOG_L3("Using default map");
        stringQueue = getDefaultMap();
    }
    else
    {
        mFile.open(mFilename);
        if (!mFile.is_open())
        {
            WARN_LOG("Cannot open file: " + mFilename + ", fall back to default map");
            stringQueue = getDefaultMap();
        }
        else
        {
            std::string line;
            while (std::getline(mFile, line))
            {
                if (line != "")
                {
                    stringQueue.push_back(line);
                }
            }
        }
    }

    Point_t mapSize = preprocessStringVector(stringQueue);
    aGameMap.clearAndResize(mapSize.x, mapSize.y);

    size_t jj = 0;

    for (std::string line : stringQueue)
    {
        for(size_t ii = 0; ii < line.length(); ++ii)
        {
            char pattern = line.at(ii);
            if (pattern == ' ')
            {
                continue;
            }
            const Terrain* const pTerrain = aGameMap.getTerrain(ii, jj);
            if (pTerrain)
            {
                // occupied
                continue;
            }
            switch (pattern)
            {
                case '+':
                {
                    // vertex
                    DEBUG_LOG_L0("Read in char '", pattern, "', adding vertex at ", Point_t{ii, jj});
                    aGameMap.addVertex(ii, jj);
                    break;
                }
                case '-':
                case '/':
                case '\\':
                {
                    // edge
                    DEBUG_LOG_L0("Read in char '", pattern, "', adding edge at ", Point_t{ii, jj});
                    aGameMap.addEdge(ii, jj, pattern);
                    break;
                }
                case '.':
                // convert ResourceTypes to numbers in ASCII, e.g. (char) 1 + '0' = '1'
                case (char)ResourceTypes::DESERT + '0':
                case (char)ResourceTypes::CLAY + '0':
                case (char)ResourceTypes::SHEEP + '0':
                case (char)ResourceTypes::WHEAT + '0':
                case (char)ResourceTypes::WOOD + '0':
                case (char)ResourceTypes::ORE + '0':
                {
                    // land
                    ResourceTypes resource = (pattern != '.') ?
                        static_cast<ResourceTypes>(pattern - '0')
                        :
                        ResourceTypes::NONE;
                    DEBUG_LOG_L0("Read in char '", pattern, "', adding land at ", Point_t{ii, jj}, " resource = " + resourceTypesToStr(resource));
                    aGameMap.addLand(ii, jj, resource);
                    break;
                }
                default:
                {
                    WARN_LOG("Unknow character '", pattern, "'(ASCII: ", (int)pattern, ") at, ", Point_t{ii, jj});
                    break;
                }
            }
        }
        ++jj;

    } // while (std::getline(mFile, line))
    mFile.close();
    INFO_LOG("Read map finished");
    return 0;
}

int MapIO::saveMap(GameMap* const aGameMap)
{
    // TODO: implement
    return 0;
}

Point_t MapIO::preprocessStringVector(std::deque<std::string>& aStringQueue)
{
    constexpr size_t VERTICAL_PADDING = 5;
    constexpr size_t HORIZONTAL_PADDING = 8;

    size_t horizontalSize = 0;
    size_t verticalSize = aStringQueue.size();

    size_t minX = std::string::npos;

    // check min X, max X, trim trailing spaces
    for (std::string& line : aStringQueue)
    {
        trimTrailingSpace(line);
        horizontalSize = std::max(horizontalSize, line.length());
        const size_t firstCharIndex = line.find_first_not_of(" ");
        minX = std::min(firstCharIndex, minX);
    }

    // add horizontal padding
    const size_t leftPaddingSize = HORIZONTAL_PADDING - minX;
    const std::string leftPadding(leftPaddingSize, ' ');
    const std::string rightPadding(HORIZONTAL_PADDING, ' ');
    for (std::string& line : aStringQueue)
    {
        line = leftPadding + line + rightPadding;
    }

    // add vertical padding
    for (size_t index = 0; index < VERTICAL_PADDING; ++index)
    {
        aStringQueue.push_front("");
        aStringQueue.push_back("");
    }

    horizontalSize = horizontalSize + HORIZONTAL_PADDING - minX + HORIZONTAL_PADDING;
    verticalSize = aStringQueue.size();

    return Point_t{horizontalSize, verticalSize};
}

std::deque<std::string> MapIO::getDefaultMap()
{
    return {
        "                                 +----------+",
        "                                /............\\",
        "                               /..............\\",
        "                   +----------+................+----------+",
        "                  /............\\............../............\\",
        "                 /..............\\............/..............\\",
        "     +----------+................+----------+................+----------+",
        "    /............\\............../............\\............../............\\",
        "   /..............\\............/..............\\............/..............\\",
        "  +................+----------+................+----------+................+",
        "   \\............../............\\............../............\\............../",
        "    \\............/..............\\............/..............\\............/",
        "     +----------+................+----------+................+----------+",
        "    /............\\............../............\\............../............\\",
        "   /..............\\............/..............\\............/..............\\",
        "  +................+----------+................+----------+................+",
        "   \\............../............\\............../............\\............../",
        "    \\............/..............\\............/..............\\............/",
        "     +----------+................+----------+................+----------+",
        "    /............\\............../............\\............../............\\",
        "   /..............\\............/..............\\............/..............\\",
        "  +................+----------+................+----------+................+",
        "   \\............../............\\............../............\\............../",
        "    \\............/..............\\............/..............\\............/",
        "     +----------+................+----------+................+----------+",
        "                 \\............../............\\............../",
        "                  \\............/..............\\............/",
        "                   +----------+................+----------+",
        "                               \\............../",
        "                                \\............/",
        "                                 +----------+"
    };
}

MapIO::MapIO(std::string aFilename) : mFilename(aFilename)
{
    // empty
}

MapIO::~MapIO()
{
    if (mFile.is_open()) {
        mFile.close();
    }
}