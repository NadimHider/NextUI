#ifndef NOCTURNE_PREVIEW_CACHE_H
#define NOCTURNE_PREVIEW_CACHE_H

#include <string.h>
#include "sdl.h"
#include "defines.h"

/* A tiny, main-thread-only LRU for save-state preview images. Three converted
 * panel-sized previews cost only a few MiB and avoid repeated image decode when
 * cycling recents. No worker lifetime or font-lock complications. */
#define PREVIEW_CACHE_COUNT 3

typedef struct {
    char path[MAX_PATH];
    SDL_Surface *surface;
    unsigned used;
} PreviewCacheEntry;

typedef struct {
    PreviewCacheEntry entries[PREVIEW_CACHE_COUNT];
    unsigned clock;
} PreviewCache;

static inline void PreviewCache_clear(PreviewCache *cache)
{
    for (int i = 0; i < PREVIEW_CACHE_COUNT; i++) {
        if (cache->entries[i].surface) SDL_FreeSurface(cache->entries[i].surface);
        cache->entries[i].surface = NULL;
        cache->entries[i].path[0] = '\0';
    }
    cache->clock = 0;
}

static inline SDL_Surface *PreviewCache_get(PreviewCache *cache, const char *path, Uint32 format)
{
    if (!path || !path[0] || strlen(path) >= MAX_PATH) return NULL;
    for (int i = 0; i < PREVIEW_CACHE_COUNT; i++) {
        PreviewCacheEntry *entry = &cache->entries[i];
        if (entry->surface && strcmp(entry->path, path) == 0) {
            entry->used = ++cache->clock;
            return entry->surface;
        }
    }

    SDL_Surface *loaded = IMG_Load(path);
    if (!loaded) return NULL;
    SDL_Surface *converted = SDL_ConvertSurfaceFormat(loaded, format, 0);
    SDL_FreeSurface(loaded);
    if (!converted) return NULL;

    int victim = 0;
    for (int i = 1; i < PREVIEW_CACHE_COUNT; i++)
        if (!cache->entries[i].surface || cache->entries[i].used < cache->entries[victim].used)
            victim = i;
    PreviewCacheEntry *entry = &cache->entries[victim];
    if (entry->surface) SDL_FreeSurface(entry->surface);
    entry->surface = converted;
    strncpy(entry->path, path, MAX_PATH - 1);
    entry->path[MAX_PATH - 1] = '\0';
    entry->used = ++cache->clock;
    return converted;
}

#endif
