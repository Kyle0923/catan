#include <iostream>
#include <fstream>
#include <curses.h>

#include "resource.hpp"

using namespace std;

int main()
{
    string line;
    ifstream map ("map.txt");
    if (map.is_open())
    {
        while ( getline (map,line) )
        {
            cout << line << '\n';
        }
        map.close();
    }
    else
    {
        cout << "Unable to open file";
        return 1;
    }
    ResourceGrid grid1(1, Point_t{20,4});
    const std::vector<Point_t> points = grid1.getAllPoints();
    int temp = points[0].y;
    for (size_t index = 0; index < points.size(); ++index)
    {
        if (points[index].y != temp)
        {
            cout << endl;
            temp = points[index].y;
        }
        cout << points[index].x << ", " << points[index].y << "\t";

    }
    cout << endl;
    system("pause");
    return 0;
}