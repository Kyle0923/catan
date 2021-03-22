#include "map_file_io.hpp"

int MapIO::readMap(GameMap& aGameMap)
{
    mFile.open(mFilename);
    if (!mFile.is_open())
    {
        ERROR_LOG("Cannot open file: ", mFilename);
        return 1;
    }
    size_t jj = 0;

    std::string line;
    while (std::getline(mFile, line))
    {
        for(size_t ii = 0; ii < line.length(); ++ii) {
            Terrain* const pTerrain = aGameMap.getTerrain(ii, jj);
            if (pTerrain)
            {
                continue;
            }
            char pattern = line.at(ii);
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
                    // blank
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