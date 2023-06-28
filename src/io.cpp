#include "io.h"

namespace io1
{

io_lib::io_lib()
{
}

gfx_lib *io_lib::getGFXLib()
{
    return &cc_gfx_lib_obj;
}

void io_lib::addTile(point p, SDL_Surface *sdlsurf)
{
    cc_gfx_lib_obj.addGFXObject( p.x()*TILEWID, p.y()*TILEHGT, sdlsurf, cc_gfx_lib_obj.getMainScreenPtr() );
}

void io_lib::printMap(grid *g)
{
    point map_loc;
    for (int y = 0; y < PHGT; ++y) {
        for (int x = 0;  x < PWID; ++x) {

            map_loc.set(y,x);

            if (g->getETile(map_loc) != NULL_e)
                addTile(map_loc,cc_gfx_lib_obj.getEntityGFXTile((int)(g->getETile(map_loc))));
            else if (g->getITile(map_loc) != NULL_i)
                addTile(map_loc,cc_gfx_lib_obj.getItemGFXTile((int)(g->getITile(map_loc))));
            else
                addTile(map_loc,cc_gfx_lib_obj.getTerrainGFXTile((int)(g->getBTile(map_loc))));
        }
    }
}

void io_lib::clearScreen()
{
    SDL_FillRect(cc_gfx_lib_obj.getMainScreenPtr(), NULL, SDL_MapRGB(cc_gfx_lib_obj.getMainScreenPtr()->format, 0, 0, 0));
}

void io_lib::printIntroScreen()
{
    cc_gfx_lib_obj.addGFXObject(0,0,cc_gfx_lib_obj.getMainMenuGFXTile(),cc_gfx_lib_obj.getMainScreenPtr());
    SDL_Flip(cc_gfx_lib_obj.getMainScreenPtr());
}


/*
string promptName()
{
	int keyp;
	int position = 1;
	string s = "";
	addChar(point(2,position),' ');

	curs_set(2);
	while((keyp = getch()) != 10 || s == "")
	{
		if (((keyp >= 65 && keyp <= 90) || (keyp >= 97 && keyp <= 122)) &&
			position < 15)
		{
			char c = (char)(keyp);
			s += c;
			addChar(point(2,position),keyp);
			position++;
		}
		else if (keyp == 8 && position > 1)
		{
			position--;
			s.erase(s.begin()+position-1,s.end());
			addChar(point(2,position),' ');
		}
		move(2,position);
	}
	curs_set(0);
	clear();
	return s;
}

*/
};


bool promptSuicide()
{
    SDL_Event key_event;
    if( SDL_WaitEvent( &key_event ) ) {
        if( key_event.type == SDL_KEYDOWN) {
            if( key_event.key.keysym.sym == SDLK_y ) {
                return true;
            }
        }
    }
    return false;
}


void promptSpace()
{
    SDL_Event key_event;
    bool hit_space = false;
    while( hit_space == false ) {
        if( SDL_WaitEvent( &key_event ) ) {
            if( key_event.type == SDL_KEYDOWN) {
                if( key_event.key.keysym.sym == SDLK_SPACE ) {
                    hit_space = true;
                }
            }
        }
    }
}

