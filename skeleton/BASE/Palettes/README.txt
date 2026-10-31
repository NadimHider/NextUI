Nocturne color palettes
----------------------

Built in: Nocturne (moonstone), Ember (warm gold), Iris (dusk violet),
and Tidal (steel blue). All four are dark-only.

Drop a version=1 .txt file in this folder to add a palette under
Settings > Appearance > Color palette. Files are sorted by name.

    version=1
    name=My night palette
    color1=0x8BD3C7FF
    color2=0x12191FFF
    color3=0x24313AFF
    color4=0xE4EBEFFF
    color5=0x071210FF
    color6=0xA7B6C0FF
    color7=0x070A0DFF

color1  Selection fill / accent
color2  Header, footer and notification surface
color3  Raised cards and unselected controls
color4  Primary text
color5  Selection text (derived automatically from color1)
color6  Secondary text and button hints
color7  Backdrop

Colors are 0xRRGGBB or 0xRRGGBBAA. UI surfaces are forced opaque.
Imported light surfaces are darkened and low-contrast text is lifted at
the settings boundary; the original file is never modified. This keeps
old NextUI palettes usable without introducing a light frontend theme.
The RG40XX H has an IPS LCD: this is a visual palette, not an OLED
power-saving feature. Adjust device brightness to your environment.
