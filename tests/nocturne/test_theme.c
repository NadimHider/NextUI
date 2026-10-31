#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "../../workspace/all/common/theme.h"

int main(void)
{
    uint32_t white = 0xFFFFFFFFU;
    uint32_t black = 0x000000FFU;
    assert(Theme_luminance(white) > 0.99);
    assert(Theme_luminance(black) == 0.0);
    assert(Theme_contrast(white, black) > 20.9);
    assert(Theme_contrast(black, white) == Theme_contrast(white, black));

    uint32_t dark = Theme_normalizeColor(2, 0xF0F0F0FFU);
    assert(Theme_luminance(dark) <= 0.035001);
    assert((dark & 255U) == 255U);
    assert(Theme_normalizeColor(2, NOCTURNE_SURFACE) == NOCTURNE_SURFACE);
    assert(Theme_luminance(Theme_normalizeColor(7, white)) <= 0.015001);

    uint32_t lifted = Theme_normalizeColor(4, 0x282828FFU);
    assert(Theme_contrast(lifted, NOCTURNE_RAISED) >= 4.499);
    assert((lifted & 255U) == 255U);
    assert(Theme_contrast(Theme_normalizeColor(6, 0x303030FFU), NOCTURNE_RAISED) >= 4.499);

    assert(Theme_contrastText(NOCTURNE_SELECTION) == NOCTURNE_ON_SELECT);
    assert(Theme_contrastText(NOCTURNE_BACKDROP) == NOCTURNE_TEXT);
    assert(Theme_contrast(Theme_contrastText(NOCTURNE_SELECTION), NOCTURNE_SELECTION) >= 4.5);
    assert((Theme_normalizeColor(1, 0x12345678U) & 255U) == 255U);

    puts("theme: ok");
    return 0;
}
