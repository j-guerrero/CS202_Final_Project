#ifndef ASSETS_H_INCLUDED
#define ASSETS_H_INCLUDED

#include <SDL2/SDL.h>
#include <cstddef>
#include <string>

//Assets (images, music, fonts, maps) are read through these functions so a Release
//build for Windows can carry them inside the executable. When an asset is not
//embedded, or on any other platform, they are read from disk as before.

#ifdef EMBEDDED_ASSETS
//Defined in assets_win.cpp
bool findEmbeddedAsset(const char* path, const void** data, size_t* size);
int embeddedAssetCount(int* total);
#else
inline bool findEmbeddedAsset(const char*, const void**, size_t*)
{ return false; }
inline int embeddedAssetCount(int* total)
{
    if(total != NULL)
    { *total = 0; }
    return 0;
}
#endif

//Opens an asset for the SDL *_RW loaders: embedded, else in the working directory,
//else next to the executable. Returns NULL if it cannot be found.
//Pass 1 as the loader's freesrc argument so the loader closes it.
inline SDL_RWops* openAsset(const std::string& path)
{
    const void* data = NULL;
    size_t size = 0;
    if(findEmbeddedAsset(path.c_str(), &data, &size))
    { return SDL_RWFromConstMem(data, (int)size); }

    //Next to the working directory, as the game has always loaded them
    SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");
    if(rw != NULL)
    { return rw; }

    //Otherwise next to the executable, so it still works when launched from elsewhere
    char* base = SDL_GetBasePath();
    if(base != NULL)
    {
        std::string beside = std::string(base) + path;
        SDL_free(base);
        rw = SDL_RWFromFile(beside.c_str(), "rb");
    }
    return rw;
}

//Reads a whole text asset into text. Returns false if it cannot be found.
inline bool readAssetText(const std::string& path, std::string& text)
{
    text.clear();
    SDL_RWops* rw = openAsset(path);
    if(rw == NULL)
    { return false; }

    char buffer[4096];
    size_t got;
    while((got = SDL_RWread(rw, buffer, 1, sizeof(buffer))) > 0)
    { text.append(buffer, got); }

    SDL_RWclose(rw);
    return true;
}

#endif
