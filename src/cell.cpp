/* function implementation of cell class */

#include "cell.h"

namespace cell1
{
// Everything here should be pretty straightforward, based on the .h file.
cell::cell()
{
    b_obj = NULL_b; // NULL means enum value 0
    e_obj = NULL_e; // ...enum value 0
    i_obj = NULL_i;
}

// get and set this cell...

back_tile cell::getBTile()
{
    return b_obj;
}

entity_tile cell::getETile()
{
    return e_obj;
}

item_tile cell::getITile()
{
    return i_obj;
}

void cell::setBTile(back_tile b)
{
    b_obj = b;
}

void cell::setETile(entity_tile e)
{
    e_obj = e;
}

void cell::setITile(item_tile i)
{
    i_obj = i;
}

// NOTE: the "grid" class contains a two dimensional array of cells...
};
