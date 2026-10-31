#!/usr/bin/env python3
"""Generate the Departure Mono bitmap table used by fbsplash.c.

Departure Mono is a pixel font designed on an 11 px grid: 7 px advance,
8 px cap height, 5 px glyph ink. A ninth row holds descenders such as the
tail of Q. Rendering at 11 px and scaling by whole numbers keeps the splash
crisp without linking SDL_ttf before the card is mounted.

Run from anywhere and paste the output into the glyph table in fbsplash.c.
"""

import os

from PIL import Image, ImageDraw, ImageFont

FONT = os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "..", "..", "..", "skeleton", "SYSTEM", "res", "DepartureMono-Regular.otf",
)
CHARS = " !-." + "0123456789" + "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

font = ImageFont.truetype(FONT, 11)


def bitmap(ch):
    cell = Image.new("L", (7, 14), 0)
    ImageDraw.Draw(cell).text((0, 0), ch, font=font, fill=255)
    rows = []
    for y in range(3, 12):  # cap height plus one descender row
        value = 0
        for x in range(1, 6):  # 5 px of ink inside the 7 px advance
            if cell.getpixel((x, y)) > 127:
                value |= 1 << (4 - (x - 1))
        rows.append(value)
    for y in range(14):
        for x in range(7):
            if cell.getpixel((x, y)) > 127 and not (3 <= y <= 11 and 1 <= x <= 5):
                raise SystemExit(f"glyph {ch!r} draws outside the 5x9 box")
    return rows


for ch in CHARS:
    rows = bitmap(ch)
    label = "space" if ch == " " else ch
    values = ",".join(f"0x{r:02x}" for r in rows)
    print(f"\t{{'{ch}', {{{{{values}}}}}}}, // {label}")
