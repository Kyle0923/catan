#ifndef INCLUDE_MAP_FILE_IO_HPP
#define INCLUDE_MAP_FILE_IO_HPP

#include <string>
#include <fstream>
#include "game_map.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "land.hpp"
#include "port.hpp"

class MapIO
{
private:
    std::string mFilename;
    std::fstream mFile;
public:
    MapIO(std::string aFilename);
    int readMap(GameMap& aGameMap);
    int saveMap(GameMap* const aGameMap);
    ~MapIO();
};

#endif /* INCLUDE_MAP_FILE_IO_HPP */
