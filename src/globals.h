#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "MersenneTwister.h"    // random number generator
#include <cassert>
#include <vector>
#include <sstream>
#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <fstream>

#define ROLL RNG.randInt
#define uint64 unsigned long long int
#define uint unsigned int

#define NUM_BACKTILE_TYPES 4
#define NUM_ENTITY_TYPES 12
#define NUM_ITEM_TYPES 3

#define FINAL_BOSS_LEVEL 17

#define SMALL_LEVEL 13

#define BOX_OF_QUEENS_LEVEL 9

#define BIG_ROOM_LEVEL 5

#define LAST_MAZE_LEVEL 4


// for the terminal...
static const int WID = 80;
static const int HGT = 24;

// for the play area.
static const int PWID = 68;
static const int PHGT = 21;


using namespace std;
// random number generator
static MTRand RNG;

enum back_tile
{
    NULL_b,
    WALL,
    FLR,
    STAIRS
};

enum entity_tile
{
    NULL_e,
    PLAYER,
    PAWN,
    ELEPHANT,
    KNIGHT,
    GLADIATOR,
    DESTROYER,
    EXECUTIONER,
    CAPTAIN,
    WARLORD,
    QUEEN,
    JUGGERNAUT
};

enum item_tile
{
    NULL_i,
    GOLD,
    JEWEL
};

// add in elephant, sniper, empress, princess, night rider ??

static const string enemy_names[12] = {
    "NULL being",
    "player",
    "pawn",
    "elephant",
    "knight",
    "ghost",
    "destroyer",
    "rook",
    "guard",
    "warden",
    "queen",
    "juggernaut"
};

static const int exp_worth[10] =
{
    1,3,3,7,75,15,5,20,30,60
};

static const int exp_levelup_points[29] =
{
    50,
    100,
    150,
    200,
    300,
    500,
    700,
    800,
    900,
    1000,
    1100,
    1200,
    1300,
    1600,
    1900,
    2100,
    2400,
    2700,
    3000,
    3300,
    3600,
    3900,
    4200,
    4500,
    4800,
    5100,
    5400,
    5700,
    6000
};

static const int small_room_npc_priority[10] =
{
    0,2,1,6,3,5,7,8,9,4
};

static const int enemy_gen_rate[17] =
{
    8,9,9,9,9,11,12,12,5,13,13,14,14,14,14,15,6
};

static const int gold_gen_rate[17] =
{
    3,4,4,4,5,6,6,6,7,7,7,7,8,8,8,9,3
};

static const int enemy_min_level[10] =
{
    1,1,1,2,14,3,1,6,10,14
};

static const int enemy_freq[10] =
{
    99,80,90,70,20,60,75,50,40,30
};

static const int gold_level_base_value[17] =
{
    5,5,6,6,12,7,8,9,10,11,12,13,14,15,16,17,20
};

#endif



