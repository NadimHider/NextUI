import os
import shutil
import sys

def organize_sd(target_drive="F:"):
    roms_dir = os.path.join(target_drive, "Roms")
    if not os.path.exists(roms_dir):
        print(f"Directory not found: {roms_dir}")
        return False

    print(f"Organizing ROMs on {roms_dir}...")

    # Canonical mapping: legacy folder -> NextUI format
    folder_mapping = {
        "famicom": "Nintendo Entertainment System (FC)",
        "nes": "Nintendo Entertainment System (FC)",
        "nesh": "Nintendo Entertainment System (FC)",
        "sfc": "Super Nintendo (SFC)",
        "snes": "Super Nintendo (SFC)",
        "snesh": "Super Nintendo (SFC)",
        "snesmsu1": "Super Nintendo (SFC)",
        "sufami": "Super Nintendo (SFC)",
        "gb": "Game Boy (GB)",
        "gbh": "Game Boy (GB)",
        "gbc": "Game Boy Color (GBC)",
        "gbch": "Game Boy Color (GBC)",
        "gba": "Game Boy Advance (GBA)",
        "gbah": "Game Boy Advance (GBA)",
        "gbav": "Game Boy Advance (GBA)",
        "megadrive": "Sega Genesis (MD)",
        "megadriveh": "Sega Genesis (MD)",
        "megadrive-japan": "Sega Genesis (MD)",
        "genh": "Sega Genesis (MD)",
        "genesis": "Sega Genesis (MD)",
        "ps": "Sony PlayStation (PS)",
        "psx": "Sony PlayStation (PS)",
        "ps1": "Sony PlayStation (PS)",
        "pce": "PC Engine (PCE)",
        "pcengine": "PC Engine (PCE)",
        "tg16": "PC Engine (PCE)",
        "tg-16": "PC Engine (PCE)",
        "tg16cd": "PC Engine (PCE)",
        "pcenginecd": "PC Engine (PCE)",
        "arcade": "Arcade (FBN)",
        "fbneo": "Arcade (FBN)",
        "mame": "MAME (MAME)",
        "gamegear": "Sega Game Gear (GG)",
        "gamegearh": "Sega Game Gear (GG)",
        "sms": "Sega Master System (SMS)",
        "mastersystem": "Sega Master System (SMS)",
        "sg-1000": "Sega Master System (SMS)",
        "segacd": "Sega CD (SEGACD)",
        "megacd": "Sega CD (SEGACD)",
        "atari2600": "Atari 2600 (A2600)",
        "atari7800": "Atari 7800 (A7800)",
        "ngpc": "Neo Geo Pocket Color (NGPC)",
        "ngp": "Neo Geo Pocket (NGP)",
        "amiga": "Amiga (PUAE)",
        "amigacd32": "Amiga (PUAE)",
        "msx": "MSX (MSX)",
        "msx2": "MSX (MSX)",
        "virtualboy": "Virtual Boy (VB)",
    }

    # Folders to delete / purge
    purge_folders = [
        "vectrex", "dreamcast", "c64", "32x", "pico8", "wonderswan", "psp", "pspminis",
        "naomi", "atomiswave", "cdi", "zmachine", "states_ra", "saves_ra", "screenshots",
        "themes", "bezels", "music", "mac", "palm", "windows", "gp32", "bk", "c16", "c128",
        "pet", "vic20", "daphne", "ios", "j2me", "odyssey", "openbor", "pc88", "pc98",
        "pcfx", "pokemini", "chip-8", "megaduck", "saturn", "st-v", "x1", "zx81", "sgfx",
        "satellaview", "supervision", "tic-80", "wasm4", "uzebox", "videopac", "x68000",
        "scv", "vircon32", "moto", "doom", "mplayer", "moonlight", "intellivision",
        "zxspectrum", "easyrpg", "scummvm", "ports", "arduboy", "atari800", "atarist",
        "atarijaguar", "channelf", "bbcmicro", "3do", "amstradcpc"
    ]

    # 1. Migrate folders
    for src_name, dst_name in folder_mapping.items():
        src_path = os.path.join(roms_dir, src_name)
        dst_path = os.path.join(roms_dir, dst_name)
        if os.path.exists(src_path) and os.path.abspath(src_path) != os.path.abspath(dst_path):
            os.makedirs(dst_path, exist_ok=True)
            moved_count = 0
            for item in os.listdir(src_path):
                s = os.path.join(src_path, item)
                d = os.path.join(dst_path, item)
                if not os.path.exists(d):
                    shutil.move(s, d)
                    moved_count += 1
            print(f"  [MIGRATE] {src_name} -> {dst_name} ({moved_count} items moved)")
            try:
                shutil.rmtree(src_path)
            except Exception as e:
                print(f"  [WARN] Could not remove {src_path}: {e}")

    # 2. Purge clutter folders
    for p_name in purge_folders:
        p_path = os.path.join(roms_dir, p_name)
        if os.path.exists(p_path):
            try:
                shutil.rmtree(p_path)
                print(f"  [PURGE] Removed clutter folder: {p_name}")
            except Exception as e:
                print(f"  [WARN] Could not remove {p_path}: {e}")

    # 3. Clean clutter files in all ROM folders
    clutter_exts = ('.xml', '.state', '.srm', '.png', '.txt')
    deleted_files = 0
    for root, dirs, files in os.walk(roms_dir):
        for f in files:
            lower = f.lower()
            if any(lower.endswith(ext) or '.state' in lower for ext in clutter_exts):
                if lower == 'gametime.txt':
                    continue
                file_path = os.path.join(root, f)
                try:
                    os.remove(file_path)
                    deleted_files += 1
                except Exception:
                    pass
    print(f"  [CLEAN] Removed {deleted_files} non-ROM clutter files (.xml, .state, .png, etc.)")
    print("Organization complete!")
    return True

if __name__ == '__main__':
    drive = sys.argv[1] if len(sys.argv) > 1 else "F:"
    organize_sd(drive)
