#include "graphics.h"

namespace graphics1
{

gfx_lib::gfx_lib()
{
    screen = NULL;
    for (int i = 0; i < 8; ++i)
        terrain_gfx_tiles[i] = NULL;
    for (int i = 0; i < 3; ++i)
        item_gfx_tiles[i] = NULL;
    for (int i = 0; i < 12; ++i)
        entity_gfx_tiles[i] = NULL;
    main_menu_gfx = NULL;
    s_message = NULL;
    font = NULL;
}

// initialize SDL
bool gfx_lib::initSDL()
{
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

    screen = SDL_SetVideoMode( SWID, SHGT, 32, SDL_SWSURFACE );

    if( screen == NULL )
        return false;

    if (TTF_Init() == -1)
        return false;

    SDL_WM_SetCaption( "Chess Dungeon", NULL );

    return true;
}

void gfx_lib::freeSDL()
{
    TTF_CloseFont( font );

    for (int i = 0; i < 8; ++i)
        SDL_FreeSurface( terrain_gfx_tiles[i]);

    for (int i = 0; i < 3; ++i)
        SDL_FreeSurface( item_gfx_tiles[i]);

    for (int i = 0; i < 12; ++i)
        SDL_FreeSurface( entity_gfx_tiles[i]);

    SDL_FreeSurface(main_menu_gfx);

    TTF_Quit();

    SDL_Quit();
}

void gfx_lib::addGFXObject( int x, int y, SDL_Surface* source, SDL_Surface* dest) {
    SDL_Rect loc;

    loc.x = x;
    loc.y = y;

    SDL_BlitSurface( source, NULL, dest, &loc);
}

void gfx_lib::addRectangle(int x, int y, int w, int h, int color) {
    SDL_Rect rectangle = {(Sint16)x,(Sint16)y,(Uint16)w,(Uint16)h};
    SDL_FillRect(screen,&rectangle,color);
}

void gfx_lib::addTranslucentRectangle(int x, int y, int w, int h, int color) {
    for (int i = 0; i < w; i+=2)
        for (int j = 0; j < h; j+=2)
            addRectangle(x+i,y+j,1,1,color);
}

void gfx_lib::drawString(string s, int x, int y)
{
    s_message = TTF_RenderText_Solid( font, &s[0],text_color );
    addGFXObject(x,y,s_message,screen);
}

SDL_Surface *gfx_lib::loadOptimizedGFXBitmap( string filename )
{
    SDL_Surface* original_img = NULL;

    SDL_Surface* optimized_img = NULL;

    original_img = SDL_LoadBMP( filename.c_str() );

    if( original_img != NULL )
    {
        optimized_img = SDL_DisplayFormat( original_img);

        SDL_FreeSurface( original_img );
    }

    return optimized_img;
}

SDL_Surface *gfx_lib::getTerrainGFXTile(int i)
{
    return terrain_gfx_tiles[i];
}

SDL_Surface *gfx_lib::getItemGFXTile(int i)
{
    return item_gfx_tiles[i];
}

SDL_Surface *gfx_lib::getEntityGFXTile(int i)
{
    return entity_gfx_tiles[i];
}

SDL_Surface *gfx_lib::getMainMenuGFXTile()
{
    return main_menu_gfx;
}

SDL_Surface *gfx_lib::getMainScreenPtr()
{
    return screen;
}

TTF_Font *gfx_lib::getFontStyle()
{
    return font;
}

bool gfx_lib::loadBitMaps()
{
    //Load the image
    terrain_gfx_tiles[0] = loadOptimizedGFXBitmap( "emptysprite.bmp" );
    terrain_gfx_tiles[1] = loadOptimizedGFXBitmap( "wallsprite.bmp" );
    terrain_gfx_tiles[2] = loadOptimizedGFXBitmap( "floorsprite.bmp" );
    terrain_gfx_tiles[3] = loadOptimizedGFXBitmap( "stairssprite.bmp" );
    terrain_gfx_tiles[4] = loadOptimizedGFXBitmap( "deadlyfloorsprite.bmp" );
    terrain_gfx_tiles[5] = loadOptimizedGFXBitmap( "deadlystairssprite.bmp" );
    terrain_gfx_tiles[6] = loadOptimizedGFXBitmap( "dangerfloorsprite.bmp" );
    terrain_gfx_tiles[7] = loadOptimizedGFXBitmap( "dangerstairssprite.bmp" );

    item_gfx_tiles[0] = loadOptimizedGFXBitmap( "emptysprite.bmp" );
    item_gfx_tiles[1] = loadOptimizedGFXBitmap( "goldsprite.bmp" );
    item_gfx_tiles[2] = loadOptimizedGFXBitmap( "orbsprite.bmp" );

    entity_gfx_tiles[0] = loadOptimizedGFXBitmap( "emptysprite.bmp" );
    entity_gfx_tiles[1] = loadOptimizedGFXBitmap( "playersprite.bmp" );
    entity_gfx_tiles[2] = loadOptimizedGFXBitmap( "pawnsprite.bmp" );
    entity_gfx_tiles[3] = loadOptimizedGFXBitmap( "elephantsprite.bmp" );
    entity_gfx_tiles[4] = loadOptimizedGFXBitmap( "knightsprite.bmp" );
    entity_gfx_tiles[5] = loadOptimizedGFXBitmap( "ghostsprite.bmp" );
    entity_gfx_tiles[6] = loadOptimizedGFXBitmap( "destroyersprite.bmp" );
    entity_gfx_tiles[7] = loadOptimizedGFXBitmap( "rooksprite.bmp" );
    entity_gfx_tiles[8] = loadOptimizedGFXBitmap( "guardsprite.bmp" );
    entity_gfx_tiles[9] = loadOptimizedGFXBitmap( "wardensprite.bmp" );
    entity_gfx_tiles[10] = loadOptimizedGFXBitmap( "queensprite.bmp" );
    entity_gfx_tiles[11] = loadOptimizedGFXBitmap( "juggernautsprite.bmp" );

    main_menu_gfx = loadOptimizedGFXBitmap("titlescreen.bmp");

    if (main_menu_gfx == NULL)
        return false;

    for (int i = 0; i < 8; ++i)
        if (terrain_gfx_tiles[i] == NULL)
            return false;

    for (int i = 0; i < 3; ++i)
        if (item_gfx_tiles[i] == NULL)
            return false;

    for (int i = 0; i < 12; ++i)
        if (entity_gfx_tiles[i] == NULL)
            return false;

    return true;
}

void gfx_lib::loadTTFFontFile()
{
    font = TTF_OpenFont( "lbrited.ttf", 10 );
}

};
