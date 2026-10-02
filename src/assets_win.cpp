// Windows-only lookup for assets embedded in the executable by assets.rc.
// Built with EMBEDDED_ASSETS defined in the Release configuration; elsewhere it
// compiles to nothing and the game reads its assets from disk.
#ifdef EMBEDDED_ASSETS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstddef>
#include <cstring>

namespace
{
    struct EmbeddedAsset
    {
        const char* path;
        int id;
    };

    const EmbeddedAsset assetTable[] =
    {
#define ASSET(id, path) { path, id },
#include "assets_list.h"
#undef ASSET
    };
}

//Counts how many of the listed assets are really embedded in this executable
int embeddedAssetCount(int* total)
{
    int found = 0;
    for(const EmbeddedAsset& asset : assetTable)
    {
        if(FindResourceA(NULL, MAKEINTRESOURCEA(asset.id), MAKEINTRESOURCEA(10)) != NULL)
        { ++found; }
    }
    if(total != NULL)
    { *total = (int)(sizeof(assetTable) / sizeof(assetTable[0])); }
    return found;
}

//Finds an embedded asset by its path relative to src/ (e.g. "images/health.png").
//The data stays valid for the life of the program.
bool findEmbeddedAsset(const char* path, const void** data, size_t* size)
{
    for(const EmbeddedAsset& asset : assetTable)
    {
        if(std::strcmp(asset.path, path) != 0)
        { continue; }

        //RT_RCDATA is MAKEINTRESOURCE(10), which is the wide-character type in a Unicode
        //build and would not match FindResourceA, so spell out the ANSI form.
        HRSRC resource = FindResourceA(NULL, MAKEINTRESOURCEA(asset.id), MAKEINTRESOURCEA(10));
        if(resource == NULL)
        { return false; }

        HGLOBAL loaded = LoadResource(NULL, resource);
        if(loaded == NULL)
        { return false; }

        *data = LockResource(loaded);
        *size = (size_t)SizeofResource(NULL, resource);
        return *data != NULL;
    }
    return false;
}

#endif
