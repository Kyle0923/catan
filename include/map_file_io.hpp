/**
 * Project: catan
 * @file map_file_io.hpp
 * @brief handles file I/O
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_MAP_FILE_IO_HPP
#define INCLUDE_MAP_FILE_IO_HPP

#include <string>
#include <fstream>
#include <deque>
#include "game_map.hpp"

class MapIO
{
private:
    std::string mFilename;
    std::fstream mFile;
    Point_t preprocessStringVector(std::deque<std::string>& aStringQueue);
    static std::deque<std::string> getDefaultMap();
public:
    MapIO(std::string aFilename);
    int readMap(GameMap& aGameMap);
    int saveMap(GameMap* const aGameMap);
    ~MapIO();
};

#endif /* INCLUDE_MAP_FILE_IO_HPP */
