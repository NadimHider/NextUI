#!/bin/sh

ROMS_DIR="/mnt/sdcard/Roms"

show2.elf --mode=simple --text="Cleaning Romset..." --timeout=2 >/dev/null 2>&1 &

migrate() {
	src="$ROMS_DIR/$1"
	dst="$ROMS_DIR/$2"
	if [ -d "$src" ] && [ "$src" != "$dst" ]; then
		mkdir -p "$dst"
		cp -rn "$src"/* "$dst"/ 2>/dev/null
		rm -rf "$src"
	fi
}

migrate "famicom" "Nintendo Entertainment System (FC)"
migrate "nes" "Nintendo Entertainment System (FC)"
migrate "nesh" "Nintendo Entertainment System (FC)"
migrate "sfc" "Super Nintendo (SFC)"
migrate "snes" "Super Nintendo (SFC)"
migrate "snesh" "Super Nintendo (SFC)"
migrate "snesmsu1" "Super Nintendo (SFC)"
migrate "sufami" "Super Nintendo (SFC)"
migrate "gb" "Game Boy (GB)"
migrate "gbh" "Game Boy (GB)"
migrate "gbc" "Game Boy Color (GBC)"
migrate "gbch" "Game Boy Color (GBC)"
migrate "gba" "Game Boy Advance (GBA)"
migrate "gbah" "Game Boy Advance (GBA)"
migrate "gbav" "Game Boy Advance (GBA)"
migrate "megadrive" "Sega Genesis (MD)"
migrate "megadriveh" "Sega Genesis (MD)"
migrate "megadrive-japan" "Sega Genesis (MD)"
migrate "genh" "Sega Genesis (MD)"
migrate "genesis" "Sega Genesis (MD)"
migrate "ps" "Sony PlayStation (PS)"
migrate "psx" "Sony PlayStation (PS)"
migrate "ps1" "Sony PlayStation (PS)"
migrate "pce" "PC Engine (PCE)"
migrate "pcengine" "PC Engine (PCE)"
migrate "tg16" "PC Engine (PCE)"
migrate "tg-16" "PC Engine (PCE)"
migrate "tg16cd" "PC Engine (PCE)"
migrate "pcenginecd" "PC Engine (PCE)"
migrate "arcade" "Arcade (FBN)"
migrate "fbneo" "Arcade (FBN)"
migrate "gamegear" "Sega Game Gear (GG)"
migrate "gamegearh" "Sega Game Gear (GG)"
migrate "sms" "Sega Master System (SMS)"
migrate "mastersystem" "Sega Master System (SMS)"
migrate "sg-1000" "Sega Master System (SMS)"
migrate "segacd" "Sega CD (SEGACD)"
migrate "megacd" "Sega CD (SEGACD)"
migrate "atari2600" "Atari 2600 (A2600)"
migrate "atari7800" "Atari 7800 (A7800)"
migrate "ngpc" "Neo Geo Pocket Color (NGPC)"
migrate "ngp" "Neo Geo Pocket (NGP)"
migrate "amiga" "Amiga (PUAE)"
migrate "amigacd32" "Amiga (PUAE)"
migrate "msx" "MSX (MSX)"
migrate "msx2" "MSX (MSX)"
migrate "virtualboy" "Virtual Boy (VB)"

for junk in \
	vectrex dreamcast c64 32x pico8 wonderswan psp pspminis naomi atomiswave \
	cdi zmachine states_ra saves_ra screenshots themes bezels music mac palm \
	windows gp32 bk c16 c128 pet vic20 daphne ios j2me odyssey openbor pc88 \
	pc98 pcfx pokemini chip-8 megaduck saturn st-v x1 zx81 sgfx satellaview \
	supervision tic-80 wasm4 uzebox videopac x68000 scv vircon32 moto doom \
	mplayer moonlight intellivision zxspectrum easyrpg scummvm ports arduboy \
	atari800 atarist atarijaguar channelf bbcmicro 3do amstradcpc; do
	if [ -d "$ROMS_DIR/$junk" ]; then
		rm -rf "$ROMS_DIR/$junk"
	fi
done

find "$ROMS_DIR" -type f \( -name "*.xml" -o -name "*.state*" -o -name "*.srm" -o -name "*.png" -o -name "*.txt" \) -delete 2>/dev/null

sync
show2.elf --mode=simple --text="Romset Cleaned!" --timeout=2 >/dev/null 2>&1
