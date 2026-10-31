#ifndef NOCTURNE_THEME_H
#define NOCTURNE_THEME_H

#include <stdint.h>
#include <math.h>

/*
 * Nocturne's semantic palette. The seven persisted CFG color slots are kept
 * for compatibility, but their meanings are now fixed:
 * 1 selection fill, 2 header/footer surface, 3 raised surface,
 * 4 primary text, 5 selection text (derived), 6 secondary text, 7 backdrop.
 *
 * This is an LCD-friendly dark palette, not a claim of OLED power savings.
 * All colors are packed RGBA. Surface/text constraints are applied at the
 * configuration boundary so old settings and community palettes stay usable.
 */
#define NOCTURNE_SELECTION  0x8BD3C7FFU
#define NOCTURNE_SURFACE    0x12191FFF
#define NOCTURNE_RAISED     0x24313AFF
#define NOCTURNE_TEXT       0xE4EBEFFFU
#define NOCTURNE_ON_SELECT  0x071210FFU
#define NOCTURNE_MUTED      0xA7B6C0FFU
#define NOCTURNE_BACKDROP   0x070A0DFFU

static inline double Theme_channel(double value)
{
    return value <= 0.04045 ? value / 12.92 : pow((value + 0.055) / 1.055, 2.4);
}

static inline double Theme_luminance(uint32_t color)
{
    double r = Theme_channel(((color >> 24) & 255) / 255.0);
    double g = Theme_channel(((color >> 16) & 255) / 255.0);
    double b = Theme_channel(((color >> 8) & 255) / 255.0);
    return 0.2126 * r + 0.7152 * g + 0.0722 * b;
}

static inline double Theme_contrast(uint32_t a, uint32_t b)
{
    double la = Theme_luminance(a), lb = Theme_luminance(b);
    return ((la > lb ? la : lb) + 0.05) / ((la < lb ? la : lb) + 0.05);
}

static inline uint32_t Theme_limitLuminance(uint32_t color, double ceiling)
{
    if (Theme_luminance(color) <= ceiling) return color | 255U;
    double lo = 0.0, hi = 1.0;
    uint32_t r = color >> 24, g = (color >> 16) & 255, b = (color >> 8) & 255;
    for (int i = 0; i < 16; i++) {
        double scale = (lo + hi) * 0.5;
        uint32_t candidate = ((uint32_t)(r * scale) << 24) |
                             ((uint32_t)(g * scale) << 16) |
                             ((uint32_t)(b * scale) << 8) | 255U;
        if (Theme_luminance(candidate) > ceiling) hi = scale;
        else lo = scale;
    }
    return ((uint32_t)(r * lo) << 24) | ((uint32_t)(g * lo) << 16) |
           ((uint32_t)(b * lo) << 8) | 255U;
}

static inline uint32_t Theme_ensureContrast(uint32_t color, uint32_t background, double minimum)
{
    color |= 255U;
    if (Theme_contrast(color, background) >= minimum) return color;
    double lo = 0.0, hi = 1.0;
    uint32_t r = color >> 24, g = (color >> 16) & 255, b = (color >> 8) & 255;
    for (int i = 0; i < 16; i++) {
        double t = (lo + hi) * 0.5;
        uint32_t candidate = ((uint32_t)(r + (255 - r) * t) << 24) |
                             ((uint32_t)(g + (255 - g) * t) << 16) |
                             ((uint32_t)(b + (255 - b) * t) << 8) | 255U;
        if (Theme_contrast(candidate, background) >= minimum) hi = t;
        else lo = t;
    }
    return ((uint32_t)(r + (255 - r) * hi) << 24) |
           ((uint32_t)(g + (255 - g) * hi) << 16) |
           ((uint32_t)(b + (255 - b) * hi) << 8) | 255U;
}

static inline uint32_t Theme_contrastText(uint32_t background)
{
    return Theme_contrast(NOCTURNE_ON_SELECT, background) >=
           Theme_contrast(NOCTURNE_TEXT, background)
           ? NOCTURNE_ON_SELECT : NOCTURNE_TEXT;
}

static inline uint32_t Theme_normalizeColor(int slot, uint32_t color)
{
    switch (slot) {
    case 2: return Theme_limitLuminance(color, 0.035);
    case 3: return Theme_limitLuminance(color, 0.070);
    case 4: return Theme_ensureContrast(color, NOCTURNE_RAISED, 4.5);
    case 6: return Theme_ensureContrast(color, NOCTURNE_RAISED, 4.5);
    case 7: return Theme_limitLuminance(color, 0.015);
    default: return color | 255U;
    }
}

#endif
