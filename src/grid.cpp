#include "grid.h"

namespace grid1
{
grid::grid()
{
}

back_tile grid::getBTile(point p)
{
    return grid_data[p.y()][p.x()].getBTile();
}

entity_tile grid::getETile(point p)
{
    return grid_data[p.y()][p.x()].getETile();
}

item_tile grid::getITile(point p)
{
    return grid_data[p.y()][p.x()].getITile();
}

void grid::setBTile(point p, back_tile b)
{
    grid_data[p.y()][p.x()].setBTile(b);
}

void grid::setETile(point p, entity_tile e)
{
    grid_data[p.y()][p.x()].setETile(e);
}

void grid::setITile(point p, item_tile i)
{
    grid_data[p.y()][p.x()].setITile(i);
}

};


