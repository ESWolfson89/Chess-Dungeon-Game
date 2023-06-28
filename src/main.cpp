#include "game.h"

using namespace game1;

int main(int argc, char* argv[])
{
    game mgame;

    if (mgame.getIOObj()->getGFXLib()->initSDL()) {

        if(mgame.getIOObj()->getGFXLib()->loadBitMaps()) {
            mgame.getIOObj()->getGFXLib()->loadTTFFontFile();

            mgame.run();

            mgame.getIOObj()->getGFXLib()->freeSDL();
        }
    }
    else {
        std::cout << "Failed to init SDL 1.2\n";
    }

    return 0;
}
