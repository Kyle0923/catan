#include "map_file_io.hpp"

int MapIO::readMap(GameMap& aGameMap, std::vector<Vertex*>& aVertices, \
                    std::vector<Edge*>& aEdges, std::vector<Land*>& aLands)
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
                    Vertex* const vertex = new Vertex(aVertices.size(), Point_t{ii, jj});
                    vertex->registerToMap(aGameMap);
                    aVertices.push_back(vertex);
                    break;
                }
                case '-':
                case '/':
                case '\\':
                {
                    // edge
                    Edge* const edge = new Edge(aEdges.size(), Point_t{ii, jj}, pattern);
                    edge->registerToMap(aGameMap);
                    aEdges.push_back(edge);
                    break;
                }
                case '.':
                // convert ResourceTypes to number in ASCII, (char) 1 + '0' = '1'
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
                        ResourceTypes::ANY;
                    Land* const land = new Land (aLands.size(), Point_t{ii, jj}, resource);
                    land->registerToMap(aGameMap);
                    aLands.push_back(land);
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