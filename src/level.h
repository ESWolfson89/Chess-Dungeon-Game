#ifndef LEVEL_H_
#define LEVEL_H_

#include "grid.h"
#include "being.h"

using namespace grid1;
using namespace being1;

namespace level1
{

static const int final_level_layout[21][40] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,3,3,3,3,3,3,3,3,3,1,1,1,1,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,1},
    {0,1,3,3,3,3,3,3,3,3,3,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,3,3,3,3,3,3,3,3,3,1,1,1,1,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,1},
    {0,1,3,3,3,3,3,3,3,3,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,3,3,3,3,3,3,3,1,1,1,1,1,6,1,1,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,7,7,7,7,7,7,1},
    {0,1,3,3,3,3,3,3,1,1,1,1,1,6,6,6,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,8,8,8,8,8,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,1,1,1,0,0,4,0,0,4,0,0,4,0,0,4,0,7,8,8,8,8,8,1},
    {0,0,5,5,5,5,5,5,0,6,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,7,8,10,10,10,8,1},
    {0,0,5,5,5,5,5,5,0,6,6,6,6,6,6,6,6,6,6,6,0,0,4,0,0,4,0,0,4,0,0,4,0,7,8,10,0,10,8,1},
    {0,0,5,5,5,5,5,5,0,6,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,7,8,10,10,10,8,1},
    {0,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,6,1,1,1,0,0,4,0,0,4,0,0,4,0,0,4,0,7,8,8,8,8,8,1},
    {0,1,1,0,0,0,0,0,0,0,1,1,1,6,6,6,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,8,8,8,8,8,1},
    {0,1,0,1,1,1,1,1,1,0,1,1,1,1,6,1,1,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,7,7,7,7,7,7,1},
    {0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,1,1,1,9,0,0,0,1,1,1,1,1,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,1},
    {0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,0,4,0,1},
    {0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


struct gold_item {
    point loc;
    int quant;
};

enum level_type
{
    SMALL_CAVE,
    SMALL_STRAIGHT_MAZE,
    SMALL_IRREGULAR_MAZE,
    BIG_CAVE,
    BIG_RECTANGLES,
    BIG_STRAIGHT_MAZE,
    BIG_IRREGULAR_MAZE,
    BIG_TOWN,
    SPECIAL_LEVEL
};

class level
{
public:
    level();

    void generate();

    grid *getGrid();

    void killBeing(being *);

    void carveMap(int,int,int);

    being *getBeing(int);

    void haveDiggersDig();

    int terrainTilesAdjVN(point,back_tile);

    int terrainTilesAdjM(point,back_tile);

    void makeCircularRooms();

    void addStairs();

    void addEnemies();

    void addOneBeing(int, point);

    being *getBeingAt(point);

    void killAllEnemies();

    int getNumEnemies();

    void addGold();

    int getGoldQuantAt(point);

    void eraseGoldAt(point);

    void eraseAllGold();

    void incLevel();

    void setLevel(int);

    void resetLevelNum();

    int getLevelNum();

    point stairsLoc();

    void doFloodFill(point p);

    void initFloodFill();

    void initFloodFillFlags();

    bool isLevelConnected();

    point startLoc();

    void addPlayerPosition();

    void addOneGoldPile(point);

    void addSpecificGoldPile(point, int);

    void loadFinalLevelLayout();

    void addJewel();

    void addBoxOfQueens();

    bool mapIsWide();

    void carveCircle(point, int, back_tile);

    void addSmallLevelRoom();

    void initTiles();

    void divideMap(int, int, int, int, bool, int, int, back_tile, int);

    void fillWithGiantRoom();

    void sealOffMazeLevel();

    void addTownLevel();

    void addOneRoom(point,point,bool);

    void fillWithRandomTiles();

    void carveTownCave(int gens, int rad1lim, int rad2lim);

    void addTownRooms();

    void addRectangularBarriers();

    gold_item *getGoldItem(int);

    int getNumGoldItems();

    void setStartLoc(point);

private:
    grid grid_obj;
    vector <being> being_objs;
    vector <gold_item> level_gold;
    int level_num;
    point stairs_loc;
    point start_loc;
    int flood_fill_flags[PHGT][PWID];
    level_type current_level_type;
};

}

#endif
