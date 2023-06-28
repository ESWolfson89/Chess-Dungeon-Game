#include "pathfinding.h"

namespace pathfind1
{

pathfind::pathfind()
{
    initPathfind();
}

void pathfind::initPathfind()
{
    point p;
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            scent_flags[i][j] = 0;
        }
    }
}

void pathfind::pfind(point source, grid *g)
{
    resetPathfind(g);
    doRecursiveScentCalculation(source, 0);
}

void pathfind::doRecursiveScentCalculation(point p, int val)
{
    if (scent_flags[p.y()][p.x()] == 0 || val < scent_flags[p.y()][p.x()])
        scent_flags[p.y()][p.x()] = val;
    else
        return;
    //doRecursiveScentCalculation(point(p.y()-1,p.x()-1),val+1);
    doRecursiveScentCalculation(point(p.y()-1,p.x()),val+1);
    //doRecursiveScentCalculation(point(p.y()-1,p.x()+1),val+1);
    doRecursiveScentCalculation(point(p.y(),p.x()-1),val+1);
    doRecursiveScentCalculation(point(p.y(),p.x()+1),val+1);
    //doRecursiveScentCalculation(point(p.y()+1,p.x()-1),val+1);
    doRecursiveScentCalculation(point(p.y()+1,p.x()),val+1);
    //doRecursiveScentCalculation(point(p.y()+1,p.x()+1),val+1);
}
// reset monster pathfinding
void pathfind::resetPathfind(grid *g)
{
    point p;
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            p.set(i,j);
            if (g->getBTile(p) != WALL)
                scent_flags[i][j] = 0;
            else
                scent_flags[i][j] = -1;
        }
    }
}
// 6 7 8
// 6 X 8
// 6 7 8

point pathfind::getLowestValueAdjacent(point p, grid *g, bool orthogonal_only)
{
    int current_min = scent_flags[p.y()][p.x()];
    point min_point = p;
    point adj;
    for (int y = -1; y <= 1; ++y) {
        for (int x = -1; x <= 1; ++x) {
            if ((y != 0 || x != 0) && (!orthogonal_only || y == 0 || x == 0)) {
                adj.set(p.y()+y,p.x()+x);
                if (scent_flags[adj.y()][adj.x()] < current_min &&
                        scent_flags[adj.y()][adj.x()] > 0 &&
                        g->getETile(adj) == NULL_e) {
                    current_min = scent_flags[adj.y()][adj.x()];
                    min_point.set(adj.y(),adj.x());
                }
            }
        }
    }
    return min_point;
}

point pathfind::getLowestValueRad2(point p, grid *g, entity_tile et)
{
    int current_min = scent_flags[p.y()][p.x()];
    point min_point = p;
    point adj;
    for (int y = -2; y <= 2; ++y) {
        for (int x = -2; x <= 2; ++x) {
            if (y != 0 || x != 0) {
                if ((et == KNIGHT && abs(y) + abs(x) == 3) ||
                        (et == GLADIATOR && abs(y) + abs(x) != 3)) {
                    adj.set(p.y()+y,p.x()+x);
                    if (adj.y() > 0 && adj.x() > 0 && adj.y() < PHGT - 1 && adj.x() < PWID - 1) {
                        if (scent_flags[adj.y()][adj.x()] < current_min &&
                                scent_flags[adj.y()][adj.x()] > 0 &&
                                g->getETile(adj) == NULL_e) {
                            current_min = scent_flags[adj.y()][adj.x()];
                            min_point.set(adj.y(),adj.x());
                        }
                    }
                }
            }
        }
    }
    return min_point;
}

};
