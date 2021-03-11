#ifndef INCLUDE_MAP_FILE_IO_HPP
#define INCLUDE_MAP_FILE_IO_HPP

#include <string>
#include <fstream>
#include "game_map.hpp"

class MapIO
{
private:
    std::string mFilename;
    std::fstream mFile;
public:
    MapIO(std::string aFilename);
    int readMap(GameMap* const aGameMap, std::vector<Terrain>& aVertex, \
                    std::vector<Terrain>& aEdge, std::vector<Terrain>& aLand);
    int saveMap(GameMap* const aGameMap);
    ~MapIO();
};

MapIO::MapIO(std::string aFilename)
{
}

MapIO::~MapIO()
{
    if (mFile.is_open()) {
        mFile.close();
    }
}

#endif /* INCLUDE_MAP_FILE_IO_HPP */
