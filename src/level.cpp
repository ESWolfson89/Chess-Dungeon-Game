#include "level.h"

namespace level1
{
level::level()
{
    resetLevelNum();
}

void level::resetLevelNum()
{
    level_num = 1;
}

void level::generate()
{
    // fill level with space tiles
    eraseAllGold();
    killAllEnemies();
    stairs_loc = point(1,1);
    start_loc = point(1,1);

    if (level_num <= 2)
        current_level_type = (level_type)(ROLL(2));
    else if (level_num <= LAST_MAZE_LEVEL)
        current_level_type = (level_type)(ROLL(1) + 3);
    else if (level_num >= 6 && level_num <= 8)
        current_level_type = (level_type)(ROLL(2) + 3);
    else if (level_num == BOX_OF_QUEENS_LEVEL)
        current_level_type = (level_type)(ROLL(3) + 3);
    else if (level_num >= 10 && level_num <= 12)
        current_level_type = (level_type)(ROLL(3) + 3);
    else if (level_num >= 14 && level_num <= 16)
        current_level_type = (level_type)(ROLL(4) + 3);
    else if (level_num == FINAL_BOSS_LEVEL)
        current_level_type = (level_type)(ROLL(3) + 3);
    else
        current_level_type = SPECIAL_LEVEL;

    if (level_num == BIG_ROOM_LEVEL)
        fillWithGiantRoom();
    else if (level_num == SMALL_LEVEL)
        addSmallLevelRoom();
    else if (current_level_type == SMALL_STRAIGHT_MAZE) {
        fillWithGiantRoom();
        divideMap(PHGT-1,(3*PWID/4)-1,0,PWID/4 - 1,false,4,4,WALL,2);
        sealOffMazeLevel();
    }
    else if (current_level_type == BIG_STRAIGHT_MAZE) {
        fillWithGiantRoom();
        divideMap(PHGT-1,PWID-1,0,0,false,4,4,WALL,2);
    }
    else if (current_level_type == BIG_RECTANGLES) {
        fillWithGiantRoom();
        addRectangularBarriers();
    }
    else {
        do {
            initFloodFillFlags();

            eraseAllGold();

            if (current_level_type != BIG_TOWN) {

                initTiles();

                haveDiggersDig();

                if (current_level_type == BIG_CAVE || current_level_type == SMALL_CAVE)
                    makeCircularRooms();
            }
            else {
                fillWithRandomTiles();
                carveTownCave(4,5,2);
                carveTownCave(3,5,-1);
                addTownRooms();
            }

            initFloodFill();
        } while(!isLevelConnected() || !mapIsWide());
    }

    if (level_num == FINAL_BOSS_LEVEL) {

        loadFinalLevelLayout();

        addPlayerPosition();

        addEnemies();

        addGold();

        addJewel();
    }
    else if (level_num != SMALL_LEVEL) {

        if (level_num == BOX_OF_QUEENS_LEVEL) {
            addPlayerPosition();
            addBoxOfQueens();
        }
        else {
            addStairs();
            addPlayerPosition();
        }

        addEnemies();

        if (current_level_type != BIG_TOWN)
            addGold();
    }
}

void level::addRectangularBarriers()
{
    for (int i = 0; i < 1000; ++i) {
        addOneRoom(point(ROLL(5) + 1,ROLL(5) + 1),point(ROLL(PHGT - 5) + 2, ROLL(PWID - 5) + 2),true);
    }

    for (int x = 0; x < PWID; ++x) {
        grid_obj.setBTile(point(1,x),WALL);
        grid_obj.setBTile(point(PHGT-2,x),WALL);
    }
}

void level::sealOffMazeLevel()
{
    point p;
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            p.set(i,j);
            if (j >= 3*PWID/4 - 1 || j <= PWID/4 - 1)
                grid_obj.setBTile(p,WALL);
        }
    }
}

void level::fillWithGiantRoom()
{
    point p;
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            p.set(i,j);
            if (i == 0 || j == 0 || i == PHGT - 1 || j == PWID - 1)
                grid_obj.setBTile(p,WALL);
            else
                grid_obj.setBTile(p,FLR);
            grid_obj.setITile(p,NULL_i);
            grid_obj.setETile(p,NULL_e);
        }
    }
}

void level::fillWithRandomTiles()
{
    point p;
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            p.set(i,j);
            if (i == 0 || j == 0 || i == PHGT - 1 || j == PWID - 1)
                grid_obj.setBTile(p,WALL);
            else {
                if (ROLL(99) >= 40)
                    grid_obj.setBTile(p,FLR);
                else
                    grid_obj.setBTile(p,WALL);
            }
            grid_obj.setITile(p,NULL_i);
            grid_obj.setETile(p,NULL_e);
        }
    }
}

void level::addTownRooms()
{
    for (int i = 0; i < 1000; ++i) {
        addOneRoom(point(ROLL(4) + 5,ROLL(4) + 5),point(ROLL(PHGT - 5) + 2, ROLL(PWID - 5) + 2),false);
    }
}

void level::carveTownCave(int gens, int rad1lim, int rad2lim)
{
    back_tile temp_tiles[PHGT][PWID];

    int nr1;
    int nr2;

    // for "gen" generations, iterate through map complex
    for (int n = 0; n < gens; n++) {

        for (int y = 0; y < PHGT; ++y) {
            for (int x = 0; x < PWID; ++x) {
                temp_tiles[y][x] = grid_obj.getBTile(point(y,x));
            }
        }

        for (int i = 1; i < PHGT - 1; ++i) {
            for (int j = 1; j < PWID - 1; ++j) {
                // number of cells 1 unit from test cell that are walls
                nr1 = 0;
                // number of cells 2 units from test cell that are walls
                nr2 = 0;

                // count nr1's
                for (int iy = -1; iy <= 1; iy++)
                    for (int ix = -1; ix <= 1; ix++)
                        if (grid_obj.getBTile(point(iy + i,ix + j)) != FLR) nr1++;

                // count nr2's
                for (int iy = -2; iy <= 2; iy++)
                    for (int ix = -2; ix <= 2; ix++)
                        if (abs(ix) == 2 || abs(iy) == 2)
                            if (grid_obj.getBTile(point(iy + i,ix + j)) != FLR) nr2++;

                // change cell based on nr1 count if test cell is wall
                if (grid_obj.getBTile(point(i,j)) == WALL) {
                    if (nr1 >= 5)
                        temp_tiles[i][j] = WALL;
                    else
                        temp_tiles[i][j] = FLR;
                }

                // change cell based on nr1 AND nr2 count if test cell is floor
                else if (grid_obj.getBTile(point(i,j)) == FLR) {
                    if (nr1 >= rad1lim || nr2 <= rad2lim)
                        temp_tiles[i][j] = WALL;
                    else
                        temp_tiles[i][j] = FLR;
                }
            }
        }

        for (int y = 1; y < PHGT - 1; ++y) {
            for (int x = 1; x < PWID - 1; ++x) {
                grid_obj.setBTile(point(y,x),temp_tiles[y][x]);
            }
        }

    }
}

void level::addOneRoom(point size, point loc, bool filled)
{
    point wall_loc;
    for (int y = -1; y <= size.y(); ++y) {
        for (int x = -1; x <= size.x(); ++x) {
            wall_loc = point(loc.y() + y, loc.x() + x);
            if (grid_obj.getBTile(wall_loc) == WALL)
                return;
            if (wall_loc.y() <= 0 || wall_loc.x() <= 0 || wall_loc.y() >= PHGT - 1 || wall_loc.x() >= PWID - 1)
                return;
        }
    }

    for (int y = 0; y < size.y(); ++y) {
        for (int x = 0; x < size.x(); ++x) {
            wall_loc = point(loc.y() + y, loc.x() + x);
            if (inBounds(wall_loc)) {
                if ((y == 0 || x == 0 || y == size.y() - 1 || x == size.x() - 1) || filled) {
                    grid_obj.setBTile(wall_loc,WALL);
                }
                else
                    addOneGoldPile(wall_loc);
            }
        }
    }

    if (!filled) {
        point p;

        do {
            p = point(ROLL(size.y()-1) + loc.y(),ROLL(size.x()-1) + loc.x());
        } while((p.y() > loc.y() && p.x() > loc.x() && p.y() < loc.y() + size.y() - 1 && p.x() < loc.x() + size.x() - 1) ||
                isAt(p,loc) ||
                isAt(p,point(loc.y() + size.y() - 1,loc.x())) ||
                isAt(p,point(loc.y(),loc.x() + size.x() - 1)) ||
                isAt(p,point(loc.y() + size.y() - 1,loc.x() + size.x() - 1)) ||
                (p.y() <= 2 || p.x() <= 2 || p.y() >= PHGT - 3 || p.x() >= PWID - 3));

        grid_obj.setBTile(p,FLR);
    }
}

void level::initTiles()
{
    point p;
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            p.set(i,j);
            grid_obj.setBTile(p,WALL);
            grid_obj.setITile(p,NULL_i);
            grid_obj.setETile(p,NULL_e);
        }
    }
}

void level::addSmallLevelRoom()
{
    initTiles();

    for (int i = 24; i <= PWID - 26; ++i)
        grid_obj.setBTile(point(10,i),FLR);

    for (int i = 8; i <= PHGT - 9; ++i) {
        grid_obj.setBTile(point(i,33),FLR);
        grid_obj.setBTile(point(i,34),FLR);
    }

    carveCircle(point(10,27),5,FLR);
    carveCircle(point(10,28),5,FLR);

    grid_obj.setBTile(point(10,27),WALL);
    grid_obj.setBTile(point(10,28),WALL);

    grid_obj.setBTile(point(10,PWID-26),STAIRS);
    stairs_loc = point(10,PWID-26);
    start_loc = point(10,22);

    for (int i = 0; i < 10; ++i) {
        addOneBeing(small_room_npc_priority[i],point(10,PWID-35+i));
        //being_objs[i].setAggroFlag(true);
    }
}

bool level::mapIsWide()
{
    int side_count = 0;
    int maxx;
    int minx;

    if (current_level_type == SMALL_STRAIGHT_MAZE || current_level_type == SMALL_IRREGULAR_MAZE ||
            current_level_type == SMALL_CAVE) {
        maxx = 3*PWID/4 - 2;
        minx = PWID/4;
    }
    else {
        maxx = PWID - 2;
        minx = 1;
    }
    for (int i = 1; i < PHGT - 1; ++i) {
        if (grid_obj.getBTile(point(i,minx)) == FLR) {
            side_count++;
            break;
        }
    }
    for (int i = 1; i < PHGT - 1; ++i) {
        if (grid_obj.getBTile(point(i,maxx)) == FLR) {
            side_count++;
            break;
        }
    }
    if (side_count >= 2)
        return true;

    return false;
}

void level::addBoxOfQueens()
{

    point p;
    point bp;

    do {
        p = point(ROLL(PHGT - 11) +5, ROLL(PWID - 11) + 5);
    } while (isWithin(p,start_loc,10));

    for (int i = -4; i <= 4; ++i) {
        for (int j = -4; j <= 4; ++j) {
            bp = point(i+p.y(),j+p.x());
            if (abs(i) == 4 || abs(j) == 4)
                grid_obj.setBTile(bp,FLR);
            else if (abs(i) == 3 || abs(j) == 3)
                grid_obj.setBTile(bp,WALL);
            else {
                grid_obj.setBTile(bp,FLR);
                if (i != 0 || j != 0)
                    addOneBeing(8,bp);
            }

            if (i == 0 && j == 0)
                grid_obj.setBTile(bp,STAIRS);
        }
    }

    switch(ROLL(3)) {
    case(0):
        grid_obj.setBTile(point(p.y(),p.x()+3),FLR);
        break;
    case(1):
        grid_obj.setBTile(point(p.y(),p.x()-3),FLR);
        break;
    case(2):
        grid_obj.setBTile(point(p.y()+3,p.x()),FLR);
        break;
    case(3):
        grid_obj.setBTile(point(p.y()-3,p.x()),FLR);
        break;
    }
}

void level::addJewel()
{
    grid_obj.setITile(point(10,PWID-4),JEWEL);
}

void level::loadFinalLevelLayout()
{
    back_tile bt;
    point tp;
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < 40; ++j) {
            tp = point(i,j+28);
            switch(final_level_layout[i][j]) {
            case(0):
                bt = FLR;
                break;
            case(1):
                bt = WALL;
                break;
            case(2):
                bt = FLR;
                addOneBeing(0,tp);
                break;
            case(3):
                bt = FLR;
                addOneGoldPile(tp);
                break;
            case(4):
                bt = FLR;
                addOneBeing(3,tp);
                break;
            case(5):
                bt = FLR;
                addOneBeing(6,tp);
                break;
            case(6):
                bt = FLR;
                addOneBeing(5,tp);
                break;
            case(7):
                bt = FLR;
                addOneBeing(7,tp);
                break;
            case(8):
                bt = FLR;
                addOneBeing(9,tp);
                break;
            case(9):
                bt = STAIRS;
                stairs_loc = tp;
                break;
            case(10):
                bt = FLR;
                addOneBeing(4,tp);
                break;
            }
            grid_obj.setBTile(tp,bt);
        }
    }
}

void level::initFloodFillFlags()
{
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            flood_fill_flags[i][j] = false;
        }
    }
}

void level::doFloodFill(point p)
{
    if (grid_obj.getBTile(p) == FLR && !flood_fill_flags[p.y()][p.x()])
        flood_fill_flags[p.y()][p.x()] = true;
    else
        return;
    doFloodFill(point(p.y()  ,p.x()+1));
    doFloodFill(point(p.y()-1,p.x()));
    doFloodFill(point(p.y(),  p.x()-1));
    doFloodFill(point(p.y()+1,p.x()));
    doFloodFill(point(p.y()-1,p.x()+1));
    doFloodFill(point(p.y()+1,p.x()-1));
    doFloodFill(point(p.y()-1,p.x()-1));
    doFloodFill(point(p.y()+1,p.x()+1));
}

void level::initFloodFill()
{
    point p;
    do {
        p = point(ROLL(PHGT-3)+1,ROLL(PWID-3)+1);
    } while (grid_obj.getBTile(p) != FLR);
    doFloodFill(p);
}

bool level::isLevelConnected()
{
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            if(flood_fill_flags[i][j] == false && grid_obj.getBTile(point(i,j)) == FLR)
                return false;
        }
    }
    return true;
}

point level::stairsLoc()
{
    return stairs_loc;
}

point level::startLoc()
{
    return start_loc;
}

void level::addGold()
{
    int maxx;

    if (level_num != FINAL_BOSS_LEVEL)
        maxx = PWID;
    else
        maxx = (PWID/2);

    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < maxx; ++j) {
            if (grid_obj.getBTile(point(i,j)) == FLR) {
                if (!isAt(point(i,j),start_loc))
                    if (gold_gen_rate[(uint)min(level_num-1,16)] > ROLL(200)) {
                        addOneGoldPile(point(i,j));
                    }
            }
        }
    }
}

void level::addOneGoldPile(point p)
{
    int value = gold_level_base_value[level_num-1];
    gold_item gold_i;
    gold_i.loc = p;
    gold_i.quant = ROLL(value*value*value*value) + value*value;
    level_gold.push_back(gold_i);
    grid_obj.setITile(p,GOLD);
}

void level::addSpecificGoldPile(point p, int q)
{
    gold_item gold_i;
    gold_i.loc = p;
    gold_i.quant = q;
    level_gold.push_back(gold_i);
    grid_obj.setITile(p,GOLD);
}

void level::addEnemies()
{
    bool gen_success;

    int enemy_selection = 0;

    int maxx;

    if (level_num != FINAL_BOSS_LEVEL)
        maxx = PWID;
    else
        maxx = 24;

    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < maxx; ++j) {
            if (grid_obj.getBTile(point(i,j)) == FLR && grid_obj.getETile(point(i,j)) == NULL_e && grid_obj.getITile(point(i,j)) == NULL_i)
                if (!isWithin(point(i,j),start_loc,4))
                    if (enemy_gen_rate[(uint)min(level_num-1,16)] > ROLL(100)) {
                        gen_success = false;
                        while (!gen_success) {
                            enemy_selection = ROLL(9);
                            if (enemy_min_level[(uint)enemy_selection] <= level_num && enemy_freq[(uint)enemy_selection] >= ROLL(99))
                                gen_success = true;
                        }
                        addOneBeing(enemy_selection,point(i,j));
                    }
        }
    }
}

being * level::getBeingAt(point p)
{
    for (uint i = 0; i < being_objs.size(); ++i) {
        if (isAt(being_objs[i].getl(),p))
            return &(being_objs[i]);
    }
    return &(being_objs[0]);
}

void level::addOneBeing(int e, point p)
{
    entity_tile etype = (entity_tile)(e+2);
    being_objs.push_back(being(etype,p));
    grid_obj.setETile(p,etype);
}

void level::addStairs()
{
    point p;
    int roller = ROLL(1);
    if (level_num == BIG_ROOM_LEVEL)
        p = point(PHGT-2,PWID-2);
    else if (current_level_type == SMALL_STRAIGHT_MAZE || current_level_type == SMALL_IRREGULAR_MAZE ||
             current_level_type == SMALL_CAVE) {
        do {
            if (roller == 0)
                p = point(ROLL(PHGT-3)+1,PWID/4);
            else
                p = point(ROLL(PHGT-3)+1,3*PWID/4 - 2);
        } while (grid_obj.getBTile(p) != FLR);
    }
    else {
        do {
            if (roller == 0)
                p = point(ROLL(PHGT-3)+1,ROLL((int)(PWID/5)-2)+1);
            else
                p = point(ROLL(PHGT-3)+1,ROLL(14) + (int)(4*PWID/5));
        } while (grid_obj.getBTile(p) != FLR);
    }
    grid_obj.setBTile(p,STAIRS);
    stairs_loc = p;
}

void level::addPlayerPosition()
{
    point p;
    if (level_num != BIG_ROOM_LEVEL) {
        if (level_num == FINAL_BOSS_LEVEL) {
            do {
                p = point(ROLL(PHGT-3)+1,ROLL(24)+1);
            } while (grid_obj.getBTile(p) != FLR);
        }
        else if (current_level_type == SMALL_STRAIGHT_MAZE || current_level_type == SMALL_IRREGULAR_MAZE ||
                 current_level_type == SMALL_CAVE) {
            do {
                if (stairs_loc.x() > PWID/2)
                    p = point(ROLL(PHGT-3)+1,PWID/4);
                else
                    p = point(ROLL(PHGT-3)+1,3*PWID/4 - 2);
            } while (grid_obj.getBTile(p) != FLR);
        }
        else {
            do {
                p = point(ROLL(PHGT-3)+1,ROLL(PWID-3)+1);
            } while (grid_obj.getBTile(p) != FLR ||
                     grid_obj.getITile(p) != NULL_i || (isWithin(p,stairs_loc,48) && level_num != BOX_OF_QUEENS_LEVEL));
        }
    }
    else
        p = point(1,1);
    start_loc = p;
}

void level::makeCircularRooms()
{
    vector <point> locs;
    for (int i = 1; i < PHGT-1; ++i)
        for (int j = 1; j < PWID-1; ++j) {
            if (terrainTilesAdjVN(point(i,j),FLR) == 2 && grid_obj.getBTile(point(i,j)) == FLR)
                locs.push_back(point(i,j));
        }

    int radius;
    int min_radius;

    min_radius = 2;

    for (uint i = 0; i < locs.size(); ++i) {
        radius = ROLL(2) + min_radius;
        carveCircle(locs[i],radius,FLR);
    }

    locs.clear();
}

void level::carveCircle(point l, int radius, back_tile bt)
{
    point p;
    for (int y = -radius; y <= radius; ++y) {
        double div = sqrt((double)(radius * radius - (y*y)));
        for (int x = -(int)div; x <= (int)div; ++x) {
            p = point(l.y()+y,l.x()+x);
            if (p.y() > 0 && p.x() > 0 && p.y() < PHGT - 1 && p.x() < PWID - 1)
                grid_obj.setBTile(p,bt);
        }
    }
}

void level::haveDiggersDig()
{
    point dir;
    int max_digs;
    int x_dig_chance_factor;
    int y_dig_chance_factor;
    int maxx,maxy,minx,miny;

    max_digs = 16;
    x_dig_chance_factor = 7;
    y_dig_chance_factor = 3;
    maxx = PWID - 2;
    maxy = PHGT - 2;
    minx = 1;
    miny = 1;


    if (current_level_type != BIG_CAVE) {
        if (current_level_type != SMALL_CAVE) {
            max_digs = 75;
            x_dig_chance_factor = 1;
            y_dig_chance_factor = 1;
        }
        if (current_level_type == SMALL_IRREGULAR_MAZE ||
                current_level_type == SMALL_CAVE) {
            maxx = (3 * PWID / 4) - 2;
            minx = PWID / 4;
            if (current_level_type == SMALL_CAVE)
                max_digs = 8;
        }
    }

    point p;

    for (int digs = 0; digs < max_digs; ++digs) {

        int ticks = 0;

        int tries = 0;

        do {
            dir = point(ROLL(2) - 1,ROLL(2) - 1);
        } while (dir.y() == 0 && dir.x() == 0 || (dir.y() != 0 && dir.x() != 0));

        if (current_level_type != SMALL_IRREGULAR_MAZE)
            p = point(ROLL(PHGT-5)+2,ROLL(PWID-5)+2);
        else
            p = point(ROLL(PHGT-5)+2,ROLL(PWID/2 - 2) + PWID/4);

        bool dig_failed = false;

        do {

            tries = 0;

            if ((p.y() == miny && dir.y() == -1) || (p.y() == maxy && dir.y() == 1)) {
                dir.sety(-dir.y());
            }

            if ((p.x() == minx && dir.x() == -1) || (p.x() == maxx && dir.x() == 1)) {
                dir.setx(-dir.x());
            }

            if (ROLL(y_dig_chance_factor) == 0 && dir.x() == 0) {
                do {
                    dir = point(ROLL(2) - 1,ROLL(2) - 1);
                    tries++;
                    if (tries > 100) {
                        dig_failed = true;
                        break;
                    }
                } while (dir.y() == 0 && dir.x() == 0 || (dir.y() != 0 && dir.x() != 0) ||
                         terrainTilesAdjVN(point(p.y()+dir.y(),p.x()+dir.x()),FLR) > 1);
            }

            if (ROLL(x_dig_chance_factor) == 0 && dir.y() == 0) {
                do {
                    dir = point(ROLL(2) - 1,ROLL(2) - 1);
                    tries++;
                    if (tries > 100) {
                        dig_failed = true;
                        break;
                    }
                } while (dir.y() == 0 && dir.x() == 0 || (dir.y() != 0 && dir.x() != 0) ||
                         terrainTilesAdjVN(point(p.y()+dir.y(),p.x()+dir.x()),FLR) > 1);
            }

            p.set(p.y()+dir.y(),p.x()+dir.x());

            if (p.y() > maxy)
                p.sety(maxy);
            if (p.y() < miny)
                p.sety(miny);
            if (p.x() > maxx)
                p.setx(maxx);
            if (p.x() < minx)
                p.setx(minx);

            grid_obj.setBTile(p,FLR);

            ticks++;

        } while (ticks < 80 && !dig_failed);

    }
}

int level::terrainTilesAdjVN(point p, back_tile bt)
{
    int num = 0;
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j)
            if (i == 0 || j == 0 && (i != 0 || j != 0))
                if (grid_obj.getBTile(point(p.y()+i,p.x()+j)) == bt)
                    num++;
    return num;
}

int level::terrainTilesAdjM(point p, back_tile bt)
{
    int num = 0;
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j)
            if (p.y() + i > 0 && p.x() + j > 0 && p.y() + i < PHGT - 1 && p.x() + j < PWID - 1)
                if (i != 0 || j != 0)
                    if (grid_obj.getBTile(point(p.y()+i,p.x()+j)) == bt)
                        num++;
    return num;
}

grid *level::getGrid()
{
    return &grid_obj;
}

being *level::getBeing(int i)
{
    return &(being_objs[i]);
}

void level::killBeing(being *captured)
{
    uint index = 0;
    for (index = 0; index < being_objs.size(); ++index) {
        if (isAt(captured->getl(),being_objs[index].getl()))
            break;
    }
    grid_obj.setETile(being_objs[index].getl(), NULL_e);
    being_objs.erase(being_objs.begin() + index);
}

void level::killAllEnemies()
{
    being_objs.clear();
}

int level::getNumEnemies()
{
    return being_objs.size();
}

int level::getGoldQuantAt(point p)
{
    for (uint i = 0; i < level_gold.size(); ++i) {
        if (isAt(level_gold[i].loc,p))
            return level_gold[i].quant;
    }
    return 0;
}

void level::eraseGoldAt(point p)
{
    uint index = 0;
    for (index = 0; index < level_gold.size(); ++index) {
        if (isAt(level_gold[index].loc,p))
            break;
    }
    grid_obj.setITile(level_gold[index].loc, NULL_i);
    level_gold.erase(level_gold.begin() + index);
}

void level::eraseAllGold()
{
    level_gold.clear();
}

void level::incLevel()
{
    level_num++;
}

void level::setLevel(int n)
{
    level_num = n;
}

int level::getLevelNum()
{
    return level_num;
}

void level::divideMap(int maxy, int maxx, int miny, int minx, bool horizontal, int w2wy, int w2wx, back_tile bt, int offset)
{
    if (maxy - miny > maxx - minx)
        horizontal = true;
    else if (maxx - minx > maxy - miny)
        horizontal = false;
    else
        horizontal = (bool)(ROLL(1));
    int dx,dy;
    int direction = ROLL(1);
    if (horizontal == false)
    {
        dx = ROLL(maxx - w2wx - minx) + (w2wx / 2) + minx;
        if ((dx < minx + (w2wx / 2))||(dx > maxx - (w2wx / 2)))
            return;
        if (direction == 0) {
            for (int p = miny; p <= maxy-offset; p++)
                grid_obj.setBTile(point(p,dx),bt);
        }
        else {
            for (int p = miny+offset; p <= maxy; p++)
                grid_obj.setBTile(point(p,dx),bt);
        }
        divideMap(maxy,dx,miny,minx,horizontal,w2wy,w2wx,bt,offset);
        divideMap(maxy,maxx,miny,dx,horizontal,w2wy,w2wx,bt,offset);
    }
    if (horizontal == true)
    {
        dy = ROLL(maxy - w2wy - miny) + (w2wy / 2) + miny;
        if ((dy < miny + (w2wy / 2))||(dy > maxy - (w2wy / 2)))
            return;
        if (direction == 0) {
            for (int q = minx; q <= maxx-offset; q++)
                grid_obj.setBTile(point(dy,q),bt);
        }
        else {
            for (int q = minx+offset; q <= maxx; q++)
                grid_obj.setBTile(point(dy,q),bt);
        }
        divideMap(dy,maxx,miny,minx,horizontal,w2wy,w2wx,bt,offset);
        divideMap(maxy,maxx,dy,minx,horizontal,w2wy,w2wx,bt,offset);
    }
}

gold_item *level::getGoldItem(int i)
{
    return &(level_gold[i]);
}

int level::getNumGoldItems()
{
    return level_gold.size();
}

void level::setStartLoc(point p)
{
    start_loc.set(p.y(),p.x());
}

};
