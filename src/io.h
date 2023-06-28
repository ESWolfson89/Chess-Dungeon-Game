#ifndef IO_H_
#define IO_H_

#include "graphics.h"
#include "grid.h"

using namespace grid1;
using namespace graphics1;

namespace io1
{

class io_lib {
public:
    io_lib();
    gfx_lib * getGFXLib();
    void addTile(point p, SDL_Surface *sdlsurf);
    void printMap(grid *g);
    void clearScreen();
    void printIntroScreen();
private:
    gfx_lib cc_gfx_lib_obj;
};

}

void promptSpace();
bool promptSuicide();


#endif




