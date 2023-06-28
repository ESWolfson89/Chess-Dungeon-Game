#ifndef GAME_H_
#define GAME_H_

#include "io.h"
#include "level.h"
#include "message.h"
#include "pathfinding.h"

using namespace io1;
using namespace level1;
using namespace message1;
using namespace pathfind1;

namespace game1
{

class game
{
public:
    game();

    void initialize();

    void run();

    void play();

    bool playerTurn();

    void moveCursor(point);

    void movePlayer();

    void reDisplay();

    void printStats();

    void captureEnemy(point);

    bool wallBlocked(back_tile);

    bool intermediateBlocked(point,point);

    point getCursorDirection(point);

    void activateEnemyAI();

    void activatePawnAI(int);

    void activateElephantAI(int);

    void activateKnightAI(int);

    void activateGladiatorAI(int);

    void activateDestroyerAI(int);

    void activateExecutionerAI(int);

    void activateCaptainAI(int);

    void activateWarlordAI(int);

    void activateQueenAI(int);

    void activateJuggernautAI(int);

    void moveEnemy(point, point, int);

    void addToPlayerGold(point);

    bool enemyCannotMoveThere(point);

    bool pathBlocked(point,point,bool,bool);

    int numPossibleMovesInDirection(point,point);

    void nextLevel();

    void resetPassableFlags();

    string int2String(int);

    void msgeAdd(string,string,string,string);

    void checkExpLevel();

    void changeMoveType();

    bool VNBlocked(point);

    bool MBlocked(point);

    bool DBlocked(point);

    void showGameHelp();

    bool checkForMate();

    void printTiles();

    void printLevelTransitionAnimation();

    void enterDebugMode();

    void mainMenu();

    void initGenerics();

    void makePassableFlagControlled(point);

    void saveGame();

    void loadGame(string);

    bool checkLoadFile(string);

    void printMessages();

    io_lib *getIOObj();

private:
    level level_obj;
    being * hero;
    bool has_moved;
    uint64 score;
    point target;
    bool has_ended;
    int passable_flags[PHGT][PWID];
    bool has_jewel;
    msgbuffer messages;
    bool has_won;
    int exp_level;
    int experience;
    bool move_type;
    bool debug_mode;
    pathfind scent_map;
    string player_name;
    io_lib io_obj;
};

}


#endif
