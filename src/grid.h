#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include "point.h"

using namespace cell1;
using namespace point1;

namespace grid1
{

// a grid is a two dimensional array of cells as defined in cell.h

class grid
{
public:
    grid();

    back_tile getBTile(point);
    void setBTile(point, back_tile);

    entity_tile getETile(point);
    void setETile(point, entity_tile);

    item_tile getITile(point);
    void setITile(point, item_tile);


private:
    cell grid_data[PHGT][PWID];
};

}

#endif
