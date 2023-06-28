#ifndef CELL_H
#define CELL_H

#include "globals.h"

namespace cell1
{
class cell
{
public:
    cell();

    // returns the tile in the background
    back_tile getBTile();
    item_tile getITile();
    entity_tile getETile();

    // set them...
    void setBTile(back_tile);
    void setITile(item_tile);
    void setETile(entity_tile);


private:
    back_tile b_obj;
    entity_tile e_obj;
    item_tile i_obj;
};

}

#endif
