#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "../../workspace/all/common/preview_cache.h"

static void make_image(const char *path, Uint32 color)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, 8, 6, 16, SDL_PIXELFORMAT_RGB565);
    assert(surface);
    assert(SDL_FillRect(surface, NULL, color) == 0);
    assert(SDL_SaveBMP(surface, path) == 0);
    SDL_FreeSurface(surface);
}

static int cached(PreviewCache *cache, const char *path)
{
    for (int i = 0; i < PREVIEW_CACHE_COUNT; i++)
        if (cache->entries[i].surface && strcmp(cache->entries[i].path, path) == 0) return 1;
    return 0;
}

int main(void)
{
    const char *a = "nocturne-preview-a.bmp";
    const char *b = "nocturne-preview-b.bmp";
    const char *c = "nocturne-preview-c.bmp";
    const char *d = "nocturne-preview-d.bmp";
    make_image(a, 0xF800);
    make_image(b, 0x07E0);
    make_image(c, 0x001F);
    make_image(d, 0xFFE0);

    PreviewCache cache = {0};
    SDL_Surface *sa = PreviewCache_get(&cache, a, SDL_PIXELFORMAT_RGB565);
    assert(sa && sa->w == 8 && sa->h == 6);
    assert(PreviewCache_get(&cache, b, SDL_PIXELFORMAT_RGB565));
    assert(PreviewCache_get(&cache, c, SDL_PIXELFORMAT_RGB565));
    assert(PreviewCache_get(&cache, a, SDL_PIXELFORMAT_RGB565) == sa); /* promote a */
    assert(PreviewCache_get(&cache, d, SDL_PIXELFORMAT_RGB565));
    assert(!cached(&cache, b)); /* b was least recently used */
    assert(cached(&cache, a) && cached(&cache, c) && cached(&cache, d));
    assert(PreviewCache_get(&cache, a, SDL_PIXELFORMAT_RGB565) == sa);
    assert(PreviewCache_get(&cache, "nocturne-missing.bmp", SDL_PIXELFORMAT_RGB565) == NULL);
    assert(PreviewCache_get(&cache, NULL, SDL_PIXELFORMAT_RGB565) == NULL);
    PreviewCache_clear(&cache);
    for (int i = 0; i < PREVIEW_CACHE_COUNT; i++) assert(!cache.entries[i].surface);
    assert(PreviewCache_get(&cache, a, SDL_PIXELFORMAT_RGB565));
    PreviewCache_clear(&cache);

    unlink(a); unlink(b); unlink(c); unlink(d);
    puts("preview cache: ok");
    return 0;
}
