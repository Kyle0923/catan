/**
 * Project: catan
 * @file map_file_io.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "map_file_io.hpp"

int MapIO::readMap(GameMap& aGameMap)
{
    mFile.open(mFilename);
    if (!mFile.is_open())
    {
        ERROR_LOG("Cannot open file: ", mFilename);
        return 1;
    }

    Point_t mapSize = validateMapBoundary();
    aGameMap.clearAndResize(mapSize.x, mapSize.y);

    size_t jj = 0;

    std::string line;
    while (std::getline(mFile, line))
    {
        for(size_t ii = 0; ii < line.length(); ++ii) {
            char pattern = line.at(ii);
            if (pattern == ' ')
            {
                continue;
            }
            Terrain* const pTerrain = aGameMap.getTerrain(ii, jj);
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
                    aGameMap.addVertex(ii, jj);
                    break;
                }
                case '-':
                case '/':
                case '\\':
                {
                    // edge
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
                    aGameMap.addLand(ii, jj, resource);
                    break;
                }
                default:
                {
                    WARN_LOG("Unknow character '", pattern, "'(ASCII: ", (int)pattern, ") at, [", ii, ", ", jj, "]");
                    break;
                }
            }
        }
        ++jj;

    } // while (std::getline(mFile, line))
    // finish reading file
    mFile.close();
    return 0;
}

int MapIO::saveMap(GameMap* const aGameMap)
{
    // TODO: implement
    return 0;
}

Point_t MapIO::validateMapBoundary()
{
    constexpr int MIN_TOP_BOTTOM_PADDING = 4;
    constexpr int MIN_LEFT_RIGHT_PADDING = 7;

    size_t horizontalSize = 0;
    size_t verticalSize = 0;

    int rc = 0;

    size_t verticalIndex = 0;
    std::string line;
    while (std::getline(mFile, line))
    {
        int startingIndex = -1;
        size_t endingIndex = 0;
        for (size_t index = 0; index < line.length(); ++index)
        {
            const char character = line.at(index);
            if (character != ' ')
            {
                endingIndex = std::max(endingIndex, index);
                if (startingIndex == -1)
                {
                    startingIndex = static_cast<int>(index);
                }
            }
        }
        horizontalSize = std::max(horizontalSize, endingIndex);

        if (startingIndex != -1)
        {
            // non empty line
            if (verticalIndex < MIN_TOP_BOTTOM_PADDING)
            {
                WARN_LOG("Please have ", MIN_TOP_BOTTOM_PADDING, " empty lines on the top of the map, current line ", (verticalIndex + 1));
                rc = 1;
            }
            if (startingIndex < MIN_LEFT_RIGHT_PADDING)
            {
                WARN_LOG("Please have ", MIN_LEFT_RIGHT_PADDING, " empty lines on the left of the map, current line ", (verticalIndex + 1));
                rc = 1;
            }
            verticalSize = verticalIndex;
        }

        ++verticalIndex;
    }

    horizontalSize += MIN_LEFT_RIGHT_PADDING;
    verticalSize += MIN_TOP_BOTTOM_PADDING;

    (rc != 0) ?
        ERROR_LOG("Map validation failed")
        :
        INFO_LOG("Map validation succeeded, map size: [", horizontalSize, ", ", verticalSize, "]");

    // reset file to the beginning
    mFile.clear();
    mFile.seekg(0, std::ios::beg);

    return Point_t{horizontalSize, verticalSize};
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