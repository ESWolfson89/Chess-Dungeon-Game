#ifndef PFIND_H
#define PFIND_H

#include "grid.h"

using namespace grid1;

namespace pathfind1
{

class pathfind
{
public:
    pathfind();
    void pfind(point, grid *);
    void doRecursiveScentCalculation(point, int);
    void resetPathfind(grid *);
    void initPathfind();
    point getLowestValueAdjacent(point,grid *,bool);
    point getLowestValueRad2(point,grid *,entity_tile);
private:
    int scent_flags[PHGT][PWID];
};

}
#endif
