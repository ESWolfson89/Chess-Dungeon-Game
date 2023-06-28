#include "game.h"

namespace game1
{
game::game()
{
    player_name = "player0";
    initGenerics();
}

io_lib *game::getIOObj()
{
    return &io_obj;
}

void game::initGenerics()
{
    score = 0ULL;
    has_ended = false;
    has_won = false;
    debug_mode = false;
    has_jewel = false;
    move_type = true;
    exp_level = 1;
    experience = 0;
    scent_map.initPathfind();
}

string game::int2String(int n)
{
    stringstream ss;
    ss << n;
    return ss.str();
}

void game::msgeAdd(string s1, string s2, string s3, string s4)
{
    string s = "";
    s.append(s1);
    s.append(s2);
    s.append(s3);
    s.append(s4);
    messages.addMessage(s);
    printMessages();
}

void game::printMessages() {
    string s;
    // clear message buffer
    io_obj.getGFXLib()->addRectangle(SWID/2,PHGT*TILEHGT,SWID/2 - 1,96,0x000000);
    // print all messages
    for (int i = 0; i < messages.getBufferSize(); ++i)
    {
        s = messages.getMessage(i);
        io_obj.getGFXLib()->drawString(s,SWID/2,PHGT*TILEHGT + (i*10));
    }
    // redisplay contents on main screen (refresh, essentially)
    SDL_Flip(io_obj.getGFXLib()->getMainScreenPtr());
}

void game::initialize() {
    initGenerics();
    level_obj.resetLevelNum();
    level_obj.generate();
    hero->setl(level_obj.startLoc());
    target = hero->getl();
    level_obj.getGrid()->setETile(hero->getl(),PLAYER);
    msgeAdd("Welcome to Chess Dungeon!","","","");
}

void game::run()
{
    hero = new being(PLAYER,point(1,1));
    string save_name = "nullname";
    SDL_Event primary_game_loop_event;
    do {
        io_obj.printIntroScreen();
        do {
            if( SDL_WaitEvent( &primary_game_loop_event ) ) {
                if( primary_game_loop_event.type == SDL_KEYDOWN) {
                    if (primary_game_loop_event.key.keysym.sym == SDLK_q) {
                        delete hero;
                        return;
                    }
                }
            }
        } while (primary_game_loop_event.key.keysym.sym != SDLK_n);
        io_obj.clearScreen();
        initialize();
        play();
    } while(1);
}

bool game::checkLoadFile(string save_name)
{
    ifstream check_file;
    save_name.append(".svg");
    check_file.open(save_name);
    if (!check_file.is_open())
        return false;
    check_file.close();
    return true;
}

// play game
void game::play() {
    resetPassableFlags();

    msgeAdd("Press '?' for help.","","","");

    reDisplay();


    do {
        if (!playerTurn()) {
            break;
        }

        if (has_moved) {
            activateEnemyAI();
            resetPassableFlags();
            reDisplay();
        }

        if (has_won) {
            score *= 16ULL;
            printStats();
            promptSpace();
            break;
        }

        // player lost
        if (has_ended) {
            msgeAdd("The ",enemy_names[(int)level_obj.getGrid()->getETile(hero->getl())]," has captured you!","");
            msgeAdd("Press [SPACE] to end the game.","","","");
            promptSpace();
            break;
        }

        if (!has_ended) {
            if (passable_flags[hero->getl().y()][hero->getl().x()] >= 2)
                msgeAdd("You are in check!","","","");

            if (passable_flags[hero->getl().y()][hero->getl().x()] == 1)
                msgeAdd("You are in the danger zone!","","","");

        }

        if(level_obj.getGrid()->getBTile(point(hero->getl())) != STAIRS) {
            if(checkForMate()) {
                msgeAdd("CHECKMATE! Game over. You have lost!","","","");
                msgeAdd("Press [SPACE] to end the game.","","","");
                has_ended = true;
                promptSpace();
                break;
            }
        }

    } while(1);

    messages.deleteAllMessages();
    level_obj.killAllEnemies();
    level_obj.eraseAllGold();
    level_obj.initTiles();
    level_obj.getGrid()->setETile(hero->getl(),NULL_e);
    io_obj.clearScreen();
}

bool game::checkForMate()
{
    int max_tile_check = exp_level;
    if (exp_level == 30)
        max_tile_check = 80;
    point loc = hero->getl();
    point cl;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dy != 0 || dx != 0) {
                for (int n = 1; n <= max_tile_check; ++n) {
                    cl = point(loc.y() + dy*n,loc.x() + dx*n);
                    if (level_obj.getGrid()->getBTile(cl) == WALL)
                        break;
                    if (passable_flags[cl.y()][cl.x()] < 2)
                        return false;
                    if (level_obj.getGrid()->getETile(cl) != NULL_e)
                        break;
                }
            }
        }
    }

    if (passable_flags[loc.y()][loc.x()] < 2)
        return false;

    return true;
}

// move player 1 cell
bool game::playerTurn() {
    target = hero->getl();

    has_moved = false;

    SDL_Event move_event;

    do {

        if( SDL_WaitEvent( &move_event ) ) {
            if( move_event.type == SDL_KEYDOWN) {
                switch(move_event.key.keysym.sym) {
                case(SDLK_KP7):
                case(SDLK_y): {
                    moveCursor(point(-1,-1));
                    break;
                }
                case(SDLK_KP8):
                case(SDLK_k): {
                    moveCursor(point(-1, 0));
                    break;
                }
                case(SDLK_KP9):
                case(SDLK_u): {
                    moveCursor(point(-1, 1));
                    break;
                }
                case(SDLK_KP4):
                case(SDLK_h): {
                    moveCursor(point(0, -1));
                    break;
                }
                case(SDLK_KP6):
                case(SDLK_l): {
                    moveCursor(point(0,  1));
                    break;
                }
                case(SDLK_KP1):
                case(SDLK_b): {
                    moveCursor(point(1, -1));
                    break;
                }
                case(SDLK_KP2):
                case(SDLK_j): {
                    moveCursor(point(1,  0));
                    break;
                }
                case(SDLK_KP3):
                case(SDLK_n): {
                    moveCursor(point(1,  1));
                    break;
                }
                case(SDLK_r):  {
                    has_moved = false;
                    target = hero->getl();
                    break;
                }
                case(SDLK_ESCAPE):   {
                    has_moved = false;
                    return false;
                }
                case(SDLK_m):        {
                    has_moved = false;
                    changeMoveType();
                    break;
                }
                case(SDLK_PERIOD):   {
                    has_moved = false;
                    nextLevel();
                    return true;
                }
                case(SDLK_F1):   {
                    has_moved = false;
                    enterDebugMode();
                    break;
                }
                case(SDLK_w):    {
                    has_moved = false;
                    if (debug_mode)
                        if (level_obj.getLevelNum() == FINAL_BOSS_LEVEL)
                            has_jewel = true;
                    break;
                }
                case(SDLK_KP5):
                case(SDLK_SPACE):   {
                    has_moved = true;
                    movePlayer();
                    break;
                }
                default:
                    has_moved = false;
                    break;
                }

            }
        }

    } while (!has_moved);

    return true;
}

void game::enterDebugMode()
{
    if (!debug_mode) {
        msgeAdd("Really enter debug mode? ('y' - confirm)","","","");
        bool debug_enter_flag = promptSuicide();
        if (debug_enter_flag) {
            msgeAdd("Game is now in debug mode - score will not count.","","","");
            debug_mode = true;
            experience = 6000;
            exp_level = 30;
            printStats();
        }
    }
}

void game::showGameHelp() {
}

void game::nextLevel()
{
    if (level_obj.getGrid()->getBTile(hero->getl()) == STAIRS || debug_mode) {
        if (level_obj.getLevelNum() < FINAL_BOSS_LEVEL) {
            level_obj.incLevel();
            level_obj.generate();
            hero->setl(level_obj.startLoc());
            resetPassableFlags();
            target = hero->getl();
            level_obj.getGrid()->setETile(hero->getl(),PLAYER);
            printLevelTransitionAnimation();
            reDisplay();
            msgeAdd("You descend to level ",int2String(level_obj.getLevelNum()),".","");
        }
        else if (has_jewel == true) {
            has_moved = false;
            has_won = true;
            msgeAdd("You have left the dungeon! You win!","","","");
            msgeAdd("Press [SPACE] to end the game.","","","");
        }
        else {
            msgeAdd("You must get the orb first!","","","");
        }
    }
    else
        msgeAdd("There are no stairs here.","","","");
}

void game::changeMoveType() {
    if (exp_level == 1) {
        msgeAdd("You cannot move multiple tiles yet!","","","");
    }
    else {
        move_type = !move_type;
        if (move_type)
            msgeAdd("Move type is now single square.","","","");
        else
            msgeAdd("Move type is now multiple squares.","","","");
    }
    target = hero->getl();
}

// here...
void game::moveCursor(point dl) {
    has_moved = false;

    // next location
    point next;

    next.set(target.y() + dl.y(), target.x() + dl.x());
    if (exp_level == 1 || move_type) {
        if (level_obj.getGrid()->getBTile(next) != WALL) {
            target.set(next.y(),next.x());
            movePlayer();
        }
        return;
    }

    if ((next.y() > 0 && next.x() > 0 && next.y() < PHGT - 1 && next.x() < PWID - 1) &&
            (next.y() == hero->getl().y() || next.x() == hero->getl().x() || abs(next.y() - hero->getl().y()) == abs(next.x() - hero->getl().x())) &&
            (!wallBlocked(level_obj.getGrid()->getBTile(next))) &&
            !intermediateBlocked(next,getCursorDirection(next)) &&
            (isWithin(hero->getl(),next,exp_level) || exp_level == 30)) {
        target.set(next.y(),next.x());
        reDisplay();
    }

}

point game::getCursorDirection(point next)
{
    point p;

    if (hero->getl().y() > next.y())
        p.sety(-1);
    else if (hero->getl().y() < next.y())
        p.sety(1);
    else
        p.sety(0);

    if (hero->getl().x() > next.x())
        p.setx(-1);
    else if (hero->getl().x() < next.x())
        p.setx(1);
    else
        p.setx(0);

    return p;
}

bool game::intermediateBlocked(point next,point rl)
{
    int num_obstructions = 0;

    if (rl.y() == 0 && rl.x() == 0)
        return false;

    point tracer = hero->getl();
    do {
        tracer.set(tracer.y() + rl.y(),tracer.x() + rl.x());
        if (tracer.y() == next.y() && tracer.x() == next.x())
            break;
        if ((int)(level_obj.getGrid()->getETile(tracer)) > 1 ||
                level_obj.getGrid()->getBTile(tracer) == WALL)
            num_obstructions++;
    } while (1);
    if (num_obstructions > 0)
        return true;
    return false;
}

bool game::wallBlocked(back_tile bt)
{
    if (bt == WALL)
        return true;
    return false;
}

void game::movePlayer()
{
    bool take_turn = true;
    if (passable_flags[target.y()][target.x()] >= 2) {
        if (isAt(target,hero->getl())) {
            msgeAdd("WARNING! Remaining in check WILL get you captured!","","","");
            msgeAdd("Press 'y' to stay anyway (else -> abort).","","","");
            //move(target.y(),target.x());
            take_turn = promptSuicide();
        }
        else
            take_turn = false;
    }

    if (passable_flags[target.y()][target.x()] == 1) {
        msgeAdd("Really move into the danger zone?","","","");
        msgeAdd("Press 'y' to confirm (else -> abort).","","","");
        //move(target.y(),target.x());
        take_turn = promptSuicide();
    }

    if (take_turn) {
        has_moved = true;
        if ((int)level_obj.getGrid()->getETile(target) > 1)
            captureEnemy(target);
        if (level_obj.getGrid()->getITile(target) == GOLD)
            addToPlayerGold(target);
        if (level_obj.getGrid()->getITile(target) == JEWEL) {
            level_obj.getGrid()->setITile(target,NULL_i);
            msgeAdd("You found the orb of power!","","","");
            msgeAdd("Leave the dungeon to win the game!","","","");
            has_jewel = true;
        }
        level_obj.getGrid()->setETile(hero->getl(),NULL_e);
        level_obj.getGrid()->setETile(target,PLAYER);
        hero->setl(target);
    }
    else {
        if (isAt(target,hero->getl()) || passable_flags[target.y()][target.x()] == 1)
            msgeAdd("Aborting move...","","","");
        else if (passable_flags[target.y()][target.x()] >= 2)
            msgeAdd("Cannot move into check!","","","");
        target = hero->getl();
        //move(target.y(),target.x());
        has_moved = false;
    }
}

void game::addToPlayerGold(point p) {
    int gold_val = level_obj.getGoldQuantAt(p);
    score += (uint64)(gold_val);
    level_obj.eraseGoldAt(p);
    msgeAdd("You found $",int2String(gold_val)," worth of gold!","");
}

void game::captureEnemy(point p) {
    msgeAdd("You captured the ",enemy_names[(int)level_obj.getGrid()->getETile(p)],"!","");
    experience += exp_worth[(int)level_obj.getGrid()->getETile(p)-2];
    checkExpLevel();
    level_obj.killBeing(level_obj.getBeingAt(p));
}

void game::checkExpLevel() {
    int initlevel = exp_level;
    int i = 0;
    for (i = initlevel; i < 30; ++i)
    {
        if (experience >= exp_levelup_points[i-1])
        {
            exp_level++;
            if (exp_level < 30)
                msgeAdd("You can now move ",int2String(exp_level)," tiles!","");
            else
                msgeAdd("You can now move unlimited tiles!","","","");

            if (exp_level == 2)
                msgeAdd("Press 'm' to toggle move type.","","","");
        }
        else
            return;
    }
}

void game::printStats() {
    io_obj.getGFXLib()->addRectangle(0,TILEHGT*PHGT,SWID/2,96, 0x000000);
    io_obj.getGFXLib()->drawString(std::string("LEVEL ") + int2String(level_obj.getLevelNum()),10,TILEHGT*PHGT);
    if (exp_level != 30) {
        io_obj.getGFXLib()->drawString(std::string("NEXT: ") + int2String(exp_levelup_points[exp_level - 1] - experience) + std::string("p"),10,TILEHGT*PHGT + 10);
        io_obj.getGFXLib()->drawString(std::string("TILES: ") + int2String(exp_level),10,TILEHGT*PHGT + 20);
    }
    else {
        io_obj.getGFXLib()->drawString(std::string("NEXT: (MAX)"),10,TILEHGT*PHGT + 10);
        io_obj.getGFXLib()->drawString(std::string("TILES: INF"),10,TILEHGT*PHGT + 20);
    }
    io_obj.getGFXLib()->drawString(std::string("GOLD: ") + int2String(score) + (has_jewel ? std::string(" >>ORB<<") : std::string("")),10,TILEHGT*PHGT + 30);
    SDL_Flip(io_obj.getGFXLib()->getMainScreenPtr());
}

void game::activateEnemyAI() {
    scent_map.pfind(hero->getl(),level_obj.getGrid());
    for (int i = 0; i < level_obj.getNumEnemies(); ++i) {
        switch(level_obj.getBeing(i)->getType()) {
        case(PAWN):
            activatePawnAI(i);
            break;
        case(ELEPHANT):
            activateElephantAI(i);
            break;
        case(KNIGHT):
            activateKnightAI(i);
            break;
        case(GLADIATOR):
            activateGladiatorAI(i);
            break;
        case(DESTROYER):
            activateDestroyerAI(i);
            break;
        case(EXECUTIONER):
            activateExecutionerAI(i);
            break;
        case(CAPTAIN):
            activateCaptainAI(i);
            break;
        case(WARLORD):
            activateWarlordAI(i);
            break;
        case(QUEEN):
            activateQueenAI(i);
            break;
        case(JUGGERNAUT):
            activateJuggernautAI(i);
            break;
        default:
            break;
        }
        if (has_ended)
            return;
    }
}

void game::makePassableFlagControlled(point p)
{
    if (passable_flags[p.y()][p.x()] < 2)
        passable_flags[p.y()][p.x()] = 2;
    else
        passable_flags[p.y()][p.x()] = 3;
}

void game::resetPassableFlags()
{
    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            passable_flags[i][j] = 0;
        }
    }

    bool warning;

    for (int i = 0; i < level_obj.getNumEnemies(); ++i) {
        point loc = level_obj.getBeing(i)->getl();
        point cl;
        point al;
        warning = false;
        switch(level_obj.getBeing(i)->getType()) {
        case(PAWN): {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    cl = point(loc.y() + y, loc.x() + x);
                    if (abs(y)+abs(x) == 2  && level_obj.getGrid()->getBTile(cl) != WALL)
                        makePassableFlagControlled(cl);
                }
            }
            break;
        }
        case(ELEPHANT): {
            for (int y = -2; y <= 2; ++y) {
                for (int x = -2; x <= 2; ++x) {
                    cl = point(loc.y() + y, loc.x() + x);
                    if (cl.y() >= 0 && cl.x() >= 0 && cl.y() <= PHGT - 1 && cl.x() <= PWID - 1) {
                        if (abs(y) + abs(x) == 4 && !pathBlocked(loc,cl,true,true))
                            makePassableFlagControlled(cl);
                    }
                }
            }
            break;
        }
        case(KNIGHT): {
            for (int y = -2; y <= 2; ++y) {
                for (int x = -2; x <= 2; ++x) {
                    cl = point(loc.y() + y, loc.x() + x);
                    if (cl.y() >= 0 && cl.x() >= 0 && cl.y() <= PHGT - 1 && cl.x() <= PWID - 1) {
                        if (abs(y)+abs(x) == 3 && level_obj.getGrid()->getBTile(cl) != WALL)
                            makePassableFlagControlled(cl);
                    }
                }
            }
            break;
        }
        case(GLADIATOR): {
            for (int y = -2; y <= 2; ++y) {
                for (int x = -2; x <= 2; ++x) {
                    cl = point(loc.y() + y, loc.x() + x);
                    if (cl.y() >= 0 && cl.x() >= 0 && cl.y() <= PHGT - 1 && cl.x() <= PWID - 1) {
                        if (y != 0 || x != 0) {
                            if (inStraightLineFrom(loc,cl) && level_obj.getGrid()->getBTile(cl) != WALL)
                                makePassableFlagControlled(cl);
                        }
                    }
                }
            }
            break;
        }
        case(DESTROYER): {
            for (int yy = -1; yy <= 1; ++yy) {
                for (int xx = -1; xx <= 1; ++xx) {
                    al = point(loc.y() + yy, loc.x() + xx);
                    if (level_obj.getGrid()->getBTile(al) != WALL && !isAt(loc,al)) {
                        for (int y = -1; y <= 1; ++y) {
                            for (int x = -1; x <= 1; ++x) {
                                warning = false;
                                if (abs(y) + abs(x) == 1) {
                                    for (int n = 0; n <= PWID - 3; ++n) {
                                        cl = point(al.y() + y*n,al.x() + x*n);
                                        if (level_obj.getGrid()->getBTile(cl) == WALL || isAt(loc,cl))
                                            break;
                                        else if (level_obj.getGrid()->getETile(cl) != NULL_e && !isAt(hero->getl(),cl) && !warning) {
                                            makePassableFlagControlled(cl);
                                            warning = true;
                                        }
                                        else if (warning && passable_flags[cl.y()][cl.x()] < 2)
                                            passable_flags[cl.y()][cl.x()] = 1;
                                        else
                                            makePassableFlagControlled(cl);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
        case(EXECUTIONER): {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    warning = false;
                    if (abs(y) + abs(x) == 1) {
                        for (int n = 1; n <= PWID - 3; ++n) {
                            cl = point(loc.y() + y*n,loc.x() + x*n);
                            if (level_obj.getGrid()->getBTile(cl) == WALL)
                                break;
                            else if (level_obj.getGrid()->getETile(cl) != NULL_e && !isAt(hero->getl(),cl) && !warning) {
                                makePassableFlagControlled(cl);
                                warning = true;
                            }
                            else if (warning && passable_flags[cl.y()][cl.x()] < 2)
                                passable_flags[cl.y()][cl.x()] = 1;
                            else
                                makePassableFlagControlled(cl);
                        }
                    }
                }
            }
            break;
        }
        case(CAPTAIN): {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    cl = point(loc.y() + y, loc.x() + x);
                    if (y != 0 || x != 0) {
                        if (level_obj.getGrid()->getBTile(cl) != WALL)
                            makePassableFlagControlled(cl);
                    }
                }
            }
            break;
        }
        case(WARLORD): {
            for (int y = -3; y <= 3; ++y) {
                for (int x = -3; x <= 3; ++x) {
                    cl = point(loc.y() + y, loc.x() + x);
                    if (cl.y() >= 0 && cl.x() >= 0 && cl.y() <= PHGT - 1 && cl.x() <= PWID - 1) {
                        if (y != 0 || x != 0) {
                            if (level_obj.getGrid()->getBTile(cl) != WALL) {
                                if (!pathBlocked(loc,cl,false,false))
                                    makePassableFlagControlled(cl);
                                else if (!pathBlocked(loc,cl,true,true) && passable_flags[cl.y()][cl.x()] < 2)
                                    passable_flags[cl.y()][cl.x()] = 1;
                            }
                        }
                    }
                }
            }
            break;
        }
        case(QUEEN): {
            for (int y = -1; y <= 1; ++y) {
                for (int x = -1; x <= 1; ++x) {
                    warning = false;
                    if (y != 0 || x != 0) {
                        for (int n = 1; n <= PWID - 3; ++n) {
                            cl = point(loc.y() + y*n,loc.x() + x*n);
                            if (level_obj.getGrid()->getBTile(cl) == WALL)
                                break;
                            else if (level_obj.getGrid()->getETile(cl) != NULL_e && !isAt(hero->getl(),cl) && !warning) {
                                makePassableFlagControlled(cl);
                                warning = true;
                            }
                            else if (warning && passable_flags[cl.y()][cl.x()] < 2)
                                passable_flags[cl.y()][cl.x()] = 1;
                            else
                                makePassableFlagControlled(cl);
                        }
                    }
                }
            }
            break;
        }
        case(JUGGERNAUT): {
            for (int y = -5; y <= 5; ++y) {
                for (int x = -5; x <= 5; ++x) {
                    cl = point(loc.y() + y, loc.x() + x);
                    if (cl.y() >= 0 && cl.x() >= 0 && cl.y() <= PHGT - 1 && cl.x() <= PWID - 1) {
                        if (y != 0 || x != 0) {
                            if (level_obj.getGrid()->getBTile(cl) != WALL) {
                                if (!pathBlocked(loc,cl,false,false))
                                    makePassableFlagControlled(cl);
                                else if (!pathBlocked(loc,cl,true,true) && passable_flags[cl.y()][cl.x()] < 2)
                                    passable_flags[cl.y()][cl.x()] = 1;
                            }
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }
}

bool game::VNBlocked(point p)
{
    if (enemyCannotMoveThere(point(p.y()+1,p.x())) &&
            enemyCannotMoveThere(point(p.y()-1,p.x())) &&
            enemyCannotMoveThere(point(p.y(),p.x()+1)) &&
            enemyCannotMoveThere(point(p.y(),p.x()-1)))
        return true;
    return false;
}

bool game::DBlocked(point p)
{
    if (enemyCannotMoveThere(point(p.y()+1,p.x()+1)) &&
            enemyCannotMoveThere(point(p.y()-1,p.x()-1)) &&
            enemyCannotMoveThere(point(p.y()+1,p.x()-1)) &&
            enemyCannotMoveThere(point(p.y()-1,p.x()+1)))
        return true;
    return false;
}

bool game::MBlocked(point p)
{
    if (VNBlocked(p) && DBlocked(p))
        return true;
    return false;
}

void game::activatePawnAI(int i)
{
    point dl;
    point nl;
    point loc = level_obj.getBeing(i)->getl();

    bool path_blocked = pathBlocked(loc,hero->getl(),false,true);

    if (!path_blocked || isWithin(loc,hero->getl(),2)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    if (abs(hero->getl().y() - loc.y()) == abs(hero->getl().x() - loc.x()) &&
            isWithin(hero->getl(),loc,1)) {
        moveEnemy(loc,hero->getl(),i);
        has_ended = true;
        return;
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {

        if (VNBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is stuck!","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),true);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is wandering...","");
            do {
                if (ROLL(1) == 0)
                    dl = point(0,ROLL(1)*2 - 1);
                else
                    dl = point(ROLL(1)*2 - 1,0);
                nl = point(loc.y()+dl.y(),loc.x()+dl.x());
            } while (enemyCannotMoveThere(nl));
        }

        moveEnemy(loc,nl,i);
    }
}

void game::activateElephantAI(int i)
{
    point dl;
    point nl;
    point cl;
    point loc = level_obj.getBeing(i)->getl();

    bool path_blocked = pathBlocked(loc,hero->getl(),false,true);
    bool path_blocked_TO = pathBlocked(loc,hero->getl(),true,true);

    if (!path_blocked || isWithin(loc,hero->getl(),4)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    for (int y = -2; y <= 2; ++y) {
        for (int x = -2; x <= 2; ++x) {
            cl = point(hero->getl().y()+y,hero->getl().x()+x);
            if (cl.y() > 0 && cl.x() > 0 && cl.y() < PHGT - 1 && cl.x() < PWID - 1) {
                if (abs(y) + abs(x) == 4 && !path_blocked_TO) {
                    if (isAt(cl,loc)) {
                        moveEnemy(loc,hero->getl(),i);
                        has_ended = true;
                        return;
                    }
                }
            }
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {

        if (VNBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is stuck!","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),true);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is wandering...","");

            int num_max_moves;

            int moved_num_times;

            do {
                if (ROLL(1) == 0)
                    dl = point(0,ROLL(1)*2 - 1);
                else
                    dl = point(ROLL(1)*2 - 1,0);
                num_max_moves = numPossibleMovesInDirection(loc,dl);
            } while (num_max_moves == 0);

            moved_num_times = ROLL(num_max_moves-1)+1;

            nl = point(loc.y() + dl.y() * moved_num_times,loc.x() + dl.x() * moved_num_times);
        }

        moveEnemy(loc,nl,i);
    }
}

void game::activateKnightAI(int i)
{
    point dl;
    point nl;
    point cl;
    point loc = level_obj.getBeing(i)->getl();

    if (isWithin(loc,hero->getl(),5)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    for (int y = -2; y <= 2; ++y) {
        for (int x = -2; x <= 2; ++x) {
            cl = point(hero->getl().y()+y,hero->getl().x()+x);
            if (cl.y() > 0 && cl.x() > 0 && cl.y() < PHGT - 1 && cl.x() < PWID - 1) {
                if (abs(cl.y() - hero->getl().y()) != abs(cl.x() - hero->getl().x()) && y != 0 && x != 0) {
                    if (isAt(cl,loc)) {
                        moveEnemy(loc,hero->getl(),i);
                        has_ended = true;
                        return;
                    }
                }
            }
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {

        nl = scent_map.getLowestValueRad2(loc,level_obj.getGrid(),KNIGHT);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is trying to pursue you.","");
            do {
                dl = point(ROLL(4)-2,ROLL(4)-2);
                nl = point(loc.y()+dl.y(),loc.x()+dl.x());
            } while (abs(nl.y() - loc.y()) == abs(nl.x() - loc.x()) || dl.y() == 0 || dl.x() == 0);

            if (nl.y() > 0 && nl.x() > 0 && nl.y() < PHGT - 1 && nl.x() < PWID - 1) {
                if (!enemyCannotMoveThere(nl)) {
                    moveEnemy(loc,nl,i);
                }
            }
        }
        else {
            moveEnemy(loc,nl,i);
        }
    }
}

void game::activateDestroyerAI(int i)
{
    point dl;
    point nl;
    point cl;
    point al;
    point dl2;
    int dist;
    point loc = level_obj.getBeing(i)->getl();

    if (isWithin(loc,hero->getl(),12) || !pathBlocked(loc,hero->getl(),false,true)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }

    for (int yy = -1; yy <= 1; ++yy) {
        for (int xx = -1; xx <= 1; ++xx) {
            al = point(loc.y() + yy,loc.x() + xx);
            if (al.y() - hero->getl().y() == 0 || al.x() - hero->getl().x() == 0) {
                if (!enemyCannotMoveThere(al) || isAt(al,loc)) {
                    dist = (int)max(abs(al.y() - hero->getl().y()),abs(al.x() - hero->getl().x()));
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (dy == 0 || dx == 0 && (dy != 0 || dx != 0)) {
                                for (int n = 1; n <= dist; ++n) {
                                    cl = point(al.y() + dy*n,al.x() + dx*n);
                                    if (level_obj.getGrid()->getETile(cl) == PLAYER) {
                                        moveEnemy(loc,hero->getl(),i);
                                        has_ended = true;
                                        return;
                                    }
                                    if (enemyCannotMoveThere(cl) && !isAt(loc,cl))
                                        break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {

        if (MBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," yells \"MOVE!\"","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),false);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is struggling to reach you.","");
            int num_max_moves;

            int moved_num_times;

            do {
                dl2 = point(loc.y() + ROLL(2) - 1, loc.x() + ROLL(2) - 1);
            }  while (enemyCannotMoveThere(dl2) && !isAt(dl2,loc));

            if (ROLL(1) == 0)
                dl = point(0,ROLL(1)*2 - 1);
            else
                dl = point(ROLL(1)*2 - 1,0);

            num_max_moves = numPossibleMovesInDirection(dl2,dl);

            moved_num_times = ROLL(num_max_moves);

            nl = point(dl2.y() + dl.y() * moved_num_times,dl2.x() + dl.x() * moved_num_times);
        }

        moveEnemy(loc,nl,i);
    }
}

void game::activateGladiatorAI(int i)
{
    point dl;
    point nl;
    point cl;
    point loc = level_obj.getBeing(i)->getl();

    if (isWithin(loc,hero->getl(),8) || !pathBlocked(loc,hero->getl(),false,true)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    for (int y = -2; y <= 2; ++y) {
        for (int x = -2; x <= 2; ++x) {
            cl = point(hero->getl().y()+y,hero->getl().x()+x);
            if (cl.y() > 0 && cl.x() > 0 && cl.y() < PHGT - 1 && cl.x() < PWID - 1) {
                if ((abs(cl.y() - hero->getl().y()) == abs(cl.x() - hero->getl().x()) || y == 0 || x == 0) &&
                        (y != 0 || x != 0)) {
                    if (isAt(cl,loc)) {
                        moveEnemy(loc,hero->getl(),i);
                        has_ended = true;
                        return;
                    }
                }
            }
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {

        nl = scent_map.getLowestValueRad2(loc,level_obj.getGrid(),GLADIATOR);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is struggling to reach you.","");
            do {
                dl = point(ROLL(4)-2,ROLL(4)-2);
                nl = point(loc.y()+dl.y(),loc.x()+dl.x());
            } while (!inStraightLineFrom(loc,nl));

            if (nl.y() > 0 && nl.x() > 0 && nl.y() < PHGT - 1 && nl.x() < PWID - 1) {
                if (!enemyCannotMoveThere(nl)) {
                    moveEnemy(loc,nl,i);
                }
            }
        }
        else
            moveEnemy(loc,nl,i);
    }
}

void game::activateExecutionerAI(int i)
{
    point dl;
    point nl;
    point cl;
    int dist;
    point loc = level_obj.getBeing(i)->getl();

    if (isWithin(loc,hero->getl(),8) || !pathBlocked(loc,hero->getl(),false,true)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    if (loc.y() - hero->getl().y() == 0 || loc.x() - hero->getl().x() == 0) {
        dist = (int)max(abs(loc.y() - hero->getl().y()),abs(loc.x() - hero->getl().x()));
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dy == 0 || dx == 0 && (dy != 0 || dx != 0)) {
                    for (int n = 1; n <= dist; ++n) {
                        cl = point(loc.y() + dy*n,loc.x() + dx*n);
                        if (level_obj.getGrid()->getETile(cl) == PLAYER) {
                            moveEnemy(loc,hero->getl(),i);
                            has_ended = true;
                            return;
                        }
                        if (enemyCannotMoveThere(cl))
                            break;
                    }
                }
            }
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {
        if (VNBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is stuck!","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),true);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is struggling to reach you.","");

            int num_max_moves;

            int moved_num_times;

            do {
                if (ROLL(1) == 0)
                    dl = point(0,ROLL(1)*2 - 1);
                else
                    dl = point(ROLL(1)*2 - 1,0);
                num_max_moves = numPossibleMovesInDirection(loc,dl);
            } while (num_max_moves == 0);

            moved_num_times = ROLL(num_max_moves-1)+1;

            nl = point(loc.y() + dl.y() * moved_num_times,loc.x() + dl.x() * moved_num_times);
        }

        moveEnemy(loc,nl,i);
    }
}

void game::activateCaptainAI(int i)
{
    point dl;
    point nl;
    point loc = level_obj.getBeing(i)->getl();

    if (!pathBlocked(loc,hero->getl(),false,true) || isWithin(loc,hero->getl(),4)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }

    if (isWithin(hero->getl(),loc,1)) {
        moveEnemy(loc,hero->getl(),i);
        has_ended = true;
        return;
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {
        if (MBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is stuck!","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),false);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is struggling to reach you.","");
            do {
                dl = point(ROLL(2)-1,ROLL(2)-1);
                nl = point(loc.y()+dl.y(),loc.x()+dl.x());
            } while ((dl.y() == 0 && dl.x() == 0) || enemyCannotMoveThere(nl));
        }

        moveEnemy(loc,nl,i);
    }
}

void game::activateWarlordAI(int i)
{
    point dl;
    point nl;
    point loc = level_obj.getBeing(i)->getl();

    bool path_blocked_LOC = pathBlocked(loc,hero->getl(),false,true);

    if (!path_blocked_LOC || isWithin(hero->getl(),loc,9)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    if (isWithin(hero->getl(),loc,3)) {
        if (!path_blocked_LOC) {
            moveEnemy(loc,hero->getl(),i);
            has_ended = true;
            return;
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {

        if (MBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," yells \"MOVE!\"","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),false);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is struggling to reach you.","");
            do {
                dl = point(ROLL(6)-3,ROLL(6)-3);
                nl = point(loc.y()+dl.y(),loc.x()+dl.x());
            } while ((dl.y() == 0 && dl.x() == 0) || pathBlocked(loc,nl,false,true));
        }

        moveEnemy(loc,nl,i);
    }
}

void game::activateQueenAI(int i)
{
    point dl;
    point nl;
    point cl;
    int dist;
    point loc = level_obj.getBeing(i)->getl();

    if (isWithin(loc,hero->getl(),12) || !pathBlocked(loc,hero->getl(),false,true)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    if (loc.y() - hero->getl().y() == 0 || loc.x() - hero->getl().x() == 0 ||
            abs(loc.y() - hero->getl().y()) == abs(loc.x() - hero->getl().x())) {
        dist = (int)max(abs(loc.y() - hero->getl().y()),abs(loc.x() - hero->getl().x()));
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dy != 0 || dx != 0) {
                    for (int n = 1; n <= dist; ++n) {
                        cl = point(loc.y() + dy*n,loc.x() + dx*n);
                        if (level_obj.getGrid()->getETile(cl) == PLAYER) {
                            moveEnemy(loc,hero->getl(),i);
                            has_ended = true;
                            return;
                        }
                        if (enemyCannotMoveThere(cl))
                            break;
                    }
                }
            }
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {
        if (MBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," yells \"MOVE!\"","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),false);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is struggling to reach you.","");
            int num_max_moves;

            int moved_num_times;

            do {
                dl = point(ROLL(2)-1,ROLL(2)-1);
                num_max_moves = numPossibleMovesInDirection(loc,dl);
            } while ((dl.y() == 0 && dl.x() == 0) || num_max_moves == 0);

            moved_num_times = ROLL(num_max_moves-1)+1;

            nl = point(loc.y() + dl.y() * moved_num_times,loc.x() + dl.x() * moved_num_times);
        }

        moveEnemy(loc,nl,i);
    }
}

void game::activateJuggernautAI(int i)
{
    point dl;
    point nl;
    point loc = level_obj.getBeing(i)->getl();

    bool path_blocked_LOC = pathBlocked(loc,hero->getl(),false,true);

    if (!path_blocked_LOC || isWithin(hero->getl(),loc,12)) {
        if (!level_obj.getBeing(i)->getAggroFlag()) {
            level_obj.getBeing(i)->setAggroFlag(true);
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," noticed you!","");
        }
    }


    if (isWithin(hero->getl(),loc,5)) {
        if (!path_blocked_LOC) {
            moveEnemy(loc,hero->getl(),i);
            has_ended = true;
            return;
        }
    }

    if (level_obj.getBeing(i)->getAggroFlag()) {

        if (MBlocked(loc)) {
            msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," yells \"MOVE!\"","");
            return;
        }

        nl = scent_map.getLowestValueAdjacent(loc,level_obj.getGrid(),false);

        if (isAt(nl,loc)) {
            if (ROLL(15) == 0)
                msgeAdd("The ",enemy_names[(int)level_obj.getBeing(i)->getType()]," is struggling to reach you.","");
            do {
                dl = point(ROLL(10)-5,ROLL(10)-5);
                nl = point(loc.y()+dl.y(),loc.x()+dl.x());
            } while ((dl.y() == 0 && dl.x() == 0) || pathBlocked(loc,nl,false,true));
        }

        moveEnemy(loc,nl,i);
    }
}

void game::moveEnemy(point loc, point next, int i)
{
    level_obj.getGrid()->setETile(loc,NULL_e);
    level_obj.getGrid()->setETile(next,level_obj.getBeing(i)->getType());
    level_obj.getBeing(i)->setl(next);
}

bool game::enemyCannotMoveThere(point p)
{
    if (level_obj.getGrid()->getBTile(p) == WALL ||
            level_obj.getGrid()->getETile(p) != NULL_e)
        return true;
    return false;
}

void game::reDisplay()
{
    printTiles();

    printStats();
    printMessages();

    SDL_Flip(io_obj.getGFXLib()->getMainScreenPtr());
}

void game::printTiles()
{
    io_obj.printMap(level_obj.getGrid());

    point map_loc;

    for (int i = 0; i < PHGT; ++i) {
        for (int j = 0; j < PWID; ++j) {
            map_loc.set(i,j);
            if (level_obj.terrainTilesAdjM(map_loc,FLR) == 0 &&
                    level_obj.terrainTilesAdjM(map_loc,STAIRS) == 0 &&
                    level_obj.getGrid()->getBTile(map_loc) == WALL)
                io_obj.getGFXLib()->addRectangle(map_loc.x()*TILEWID,map_loc.y()*TILEHGT,TILEWID,TILEHGT,0x000000);
            if (passable_flags[i][j] >= 2     && level_obj.getGrid()->getITile(map_loc) == NULL_i &&
                    level_obj.getGrid()->getETile(map_loc) == NULL_e)
                io_obj.addTile(map_loc,io_obj.getGFXLib()->getTerrainGFXTile((int)(level_obj.getGrid()->getBTile(map_loc)) + 2));
            if (passable_flags[i][j] == 1     && level_obj.getGrid()->getITile(map_loc) == NULL_i &&
                    level_obj.getGrid()->getETile(map_loc) == NULL_e)
                io_obj.addTile(map_loc,io_obj.getGFXLib()->getTerrainGFXTile((int)(level_obj.getGrid()->getBTile(map_loc)) + 4));
        }
    }

    if (move_type)
        io_obj.getGFXLib()->addTranslucentRectangle(target.x()*18,target.y()*32,18,32,0x6F6F6F);
    else
        io_obj.getGFXLib()->addTranslucentRectangle(target.x()*18,target.y()*32,18,32,0xAFAFAF);
}

void game::printLevelTransitionAnimation() {
}

int game::numPossibleMovesInDirection(point p, point dl)
{
    int num = -1;
    point nl = p;
    do {
        nl = point(nl.y() + dl.y(), nl.x() + dl.x());
        num++;
    } while (!enemyCannotMoveThere(nl));
    return num;
}

// bresenham's line algorithm
bool game::pathBlocked(point s, point e, bool terrain_only, bool end_included)
{
    int my = e.y();
    int mx = e.x();

    int py = s.y();
    int px = s.x();

    if (my < 1 || mx < 1 || my > PHGT - 2 || mx > PWID - 2)
        return true;

    int dy = abs(py - my);
    int dx = abs(px - mx);

    int sy, sx;

    if (my < py )
        sy = 1;
    else
        sy = -1;

    if ( mx < px )
        sx = 1;
    else
        sx = -1;

    int error = dx - dy;
    int error2;

    while (mx != px || my != py) {
        if (end_included || mx != e.x() || my != e.y()) {
            if (!(terrain_only && level_obj.getGrid()->getETile(point(my,mx)) != NULL_e))
                if (enemyCannotMoveThere(point(my,mx)) &&
                        level_obj.getGrid()->getETile(point(my,mx)) != PLAYER)
                    return true;
        }
        error2 = 2 * error;
        if (error2 > -dy)
        {
            error -= dy;
            mx += sx;
        }
        if (error2 < dx)
        {
            error += dx;
            my += sy;
        }
    }
    return false;
}

void game::loadGame(string save_file_player_name)
{
    save_file_player_name.append(".svg");

    initGenerics();
    level_obj.resetLevelNum();

    int debug_mode_temp;
    int has_jewel_temp;
    int move_type_temp;
    int start_locy, start_locx;
    int current_level;
    int terrain_temp;
    int num_total_gold_items;
    int gold_quant_temp;
    int gold_locy, gold_locx;
    int hero_locy, hero_locx;
    int num_total_enemies;
    int enemy_type_temp;
    int enemy_locy, enemy_locx;
    int aggro_flag_temp;

    ifstream save_file_read;

    save_file_read.open(save_file_player_name);

    save_file_read >> player_name;
    save_file_read >> debug_mode_temp >> has_jewel_temp;
    save_file_read >> score >> experience >> exp_level >> move_type_temp;
    save_file_read >> current_level >> start_locy >> start_locx;

    debug_mode = (bool)(debug_mode_temp);
    has_jewel  = (bool)(has_jewel_temp);
    move_type  = (bool)(move_type_temp);
    level_obj.setLevel(current_level);
    level_obj.setStartLoc(point(start_locy,start_locx));

    for (int y = 0; y < PHGT; ++y) {
        for (int x = 0; x < PWID; ++x) {
            save_file_read >> terrain_temp;
            level_obj.getGrid()->setBTile(point(y,x),(back_tile)(terrain_temp));
        }
    }

    save_file_read >> num_total_gold_items;

    for (int i = 0; i < num_total_gold_items; ++i) {
        save_file_read >> gold_locy >> gold_locx >> gold_quant_temp;
        level_obj.addSpecificGoldPile(point(gold_locy,gold_locx),gold_quant_temp);
    }

    if (current_level == FINAL_BOSS_LEVEL && !has_jewel)
        level_obj.addJewel();

    save_file_read >> num_total_enemies >> hero_locy >> hero_locx;

    hero->setl(point(hero_locy,hero_locx));
    level_obj.getGrid()->setETile(hero->getl(),PLAYER);

    for (int i = 0; i < num_total_enemies; ++i) {
        save_file_read >> enemy_type_temp >> aggro_flag_temp >> enemy_locy >> enemy_locx;
        level_obj.addOneBeing(enemy_type_temp - 2,point(enemy_locy,enemy_locx));
        level_obj.getBeing(i)->setAggroFlag((bool)(aggro_flag_temp));
    }

    target = hero->getl();

    save_file_read.close();

    remove(&save_file_player_name[0]);

    msgeAdd("Welcome back to Chess Dungeon!","","","");

    return;
}

// things to save
// level class:
// level num
// start_loc
// game class:
// call initGenerics()
// debug_mode
// has_jewel
// move_type
// exp_level
// experience
// score
// call inittiles
// entire map (terrain only)
// # - wall
// . - floor
// > - stairs
// gold pieces (as a list in file coords/values)
// jewel loc (as a coord in file)
// player loc info (example)
// 1 10 10 (npc type 1, loc pt 10,10)
// list of npcs and locations in order appearing in vector
// examples:
// 2 1 5 10 // pawn, aggroed, loc 5,10 yx
// 4 0 10 40 // knight, not aggroed loc 10 40
// 8 0 1 50// guard, not aggroed loc 1 50

void game::saveGame()
{
    string save_file_player_name = player_name;

    save_file_player_name.append(".svg");

    ofstream save_file_write;

    save_file_write.open(save_file_player_name);

    save_file_write << player_name << "\n" << "\n";

    save_file_write << (int)debug_mode << " "
                    << (int)has_jewel << "\n";

    save_file_write << score << " " << experience << " " << exp_level << " " << (int)move_type << "\n";

    save_file_write << level_obj.getLevelNum() << " " << level_obj.startLoc().y() << " " << level_obj.startLoc().x() << "\n" << "\n";

    for (int y = 0; y < PHGT; ++y) {
        for (int x = 0; x < PWID; ++x) {
            if (x == 0 || y == 0 || x == PWID - 1 || y == PHGT - 1)
                save_file_write << 1 << " ";
            else
                save_file_write << (int)level_obj.getGrid()->getBTile(point(y,x)) << " ";
        }
        save_file_write << "\n";
    }

    save_file_write << "\n";

    save_file_write << level_obj.getNumGoldItems() << "\n" << "\n";

    for (int i = 0; i < level_obj.getNumGoldItems(); ++i) {
        save_file_write << level_obj.getGoldItem(i)->loc.y() << " "
                        << level_obj.getGoldItem(i)->loc.x() << " "
                        << level_obj.getGoldItem(i)->quant << "\n";
    }

    save_file_write << "\n";

    save_file_write << level_obj.getNumEnemies() << "\n" << "\n";

    save_file_write << hero->getl().y() << " " << hero->getl().x() << "\n" << "\n";


    for (int i = 0; i < level_obj.getNumEnemies(); ++i) {
        save_file_write << (int)level_obj.getBeing(i)->getType() << " "
                        << (int)level_obj.getBeing(i)->getAggroFlag() << " "
                        << level_obj.getBeing(i)->getl().y() << " "
                        << level_obj.getBeing(i)->getl().x() << "\n";
    }

    save_file_write << "\n";

    save_file_write.close();

    return;
}

};
