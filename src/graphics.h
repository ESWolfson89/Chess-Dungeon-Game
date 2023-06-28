#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "globals.h"

#define SWID 1224
#define SHGT 712

#define TILEWID 18
#define TILEHGT 32

static SDL_Color text_color = {255,255,255};

namespace graphics1
{

class gfx_lib
{

public:
    gfx_lib();
    bool initSDL();
    void freeSDL();
    void addGFXObject(int, int, SDL_Surface*, SDL_Surface*);
    void addRectangle(int,int,int,int,int);
    void addTranslucentRectangle(int,int,int,int,int);
    void drawString(string,int,int);
    SDL_Surface *loadOptimizedGFXBitmap(string);
    SDL_Surface *getTerrainGFXTile(int);
    SDL_Surface *getItemGFXTile(int);
    SDL_Surface *getEntityGFXTile(int);
    SDL_Surface *getMainMenuGFXTile();
    SDL_Surface *getMainScreenPtr();
    TTF_Font *getFontStyle();
    bool loadBitMaps();
    void loadTTFFontFile();
private:
    SDL_Surface *screen;
    SDL_Surface *terrain_gfx_tiles[8];
    SDL_Surface *item_gfx_tiles[3];
    SDL_Surface *entity_gfx_tiles[12];
    SDL_Surface *main_menu_gfx;
    SDL_Surface *s_message;
    TTF_Font *font;
};

}

#endif
