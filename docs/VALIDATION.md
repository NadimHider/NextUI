# Validation

This page records the build, binary, card, and device checks for Nocturne v1.0.1.

## Build provenance

| Item | Value |
| --- | --- |
| Release | Nocturne v1.0.1 |
| Binary commit | `12927c60` |
| H700 workflow | [41165558582](https://github.com/NadimHider/nocturne/actions/runs/41165558582), success |
| Full CI | [41165558579](https://github.com/NadimHider/nocturne/actions/runs/41165558579), success |
| Device | Anbernic RG40XX H |
| Card | exFAT, label `Roms` |
| Validation date | 2026-10-31 |

The release tag includes the documentation update that follows the binary commit. The binaries in the release archive were built from `12927c60`.

## Automated checks

### Host policy tests

Command:

```sh
make test-nocturne
```

Result: pass.

- Theme normalization: pass
- Switcher navigation and removal policy: pass
- Preview cache promotion, eviction, and clear: pass

The tests cover luminance and contrast normalization, selection text derivation, recent-list navigation, two-step removal, and the three-entry LRU preview cache.

### Full CI

Run [41165558579](https://github.com/NadimHider/nocturne/actions/runs/41165558579) completed successfully.

- 84 core build jobs passed.
- The aggregate build job passed.
- The package, upload, and attestation stages passed.

### H700 build

Run [41165558582](https://github.com/NadimHider/nocturne/actions/runs/41165558582) completed successfully.

- The H700 cross-build passed.
- The update archive was packaged and uploaded.
- The archive was downloaded and installed for validation.

## Binary verification

The installed binaries are aarch64 ELF files. Their `DT_NEEDED` dependency lists match the previous build.

```text
nextui.elf       CAD17B81D62BEDC2E37DF00C5BDF03D849C70F8008AB4B61BD9838841C4ADF82
minarch.elf      5B87BE571189DBF645E211D9CBB28DFE5AA597992772B463E9E46992B4EA774F
keymon.elf       2FDD6E090DD8D4C0A624F7C20F28B83EA2FFAADCBC87F19226B136C6A217E025
nextval.elf      A6021FFCEE19D9B1FD0D450CC30D66FE3C4D8A2C96DBD8647A4996F602C0A7F7
syncsettings.elf D3A6C0AA1C3CD338DA5CB5B167CF5758A9253BD7E6E9C333F90C6066A193C673
settings.elf     92E451285B6191143A67E1B823EE013DD512364E0B0AE0047245CB89C82E4127
ledcontrol.elf   E85AFE7448A8059D3E3BF62E6DE53463954FDF575C91E62B6B6E9DC1F3C392E0
```

| Check | Result |
| --- | --- |
| ELF class | 64-bit |
| Machine | AArch64 (183) |
| Dependency changes | None |
| Frontend hash matches artifact | Pass |
| MinArch hash matches artifact | Pass |
| Settings hash matches artifact | Pass |

## Card verification

| Check | Result |
| --- | --- |
| `.system`, `.userdata`, `Tools`, `Emus`, `Bios`, `Saves`, `Shaders`, `Roms` at card root | Pass |
| `.system/h700/bin` contains the release binaries | Pass |
| `Tools/h700/Settings.pak` contains the release Settings binary | Pass |
| `Tools/h700/LedControl.pak` contains the release LED Control binary | Pass |
| `Tools/h700/CleanRomset.pak` contains the release cleanup script | Pass |
| `defaultView=0` in `minuisettings.txt` | Pass |
| Built-in palettes present, including the six new palettes | Pass |
| Boot-critical files present | Pass |
| Zero-byte files under `.system` | None |
| `chkdsk` file system scan | No problems |
| Dirty bit | Not set |
| Volume cache | Flushed before eject |

The six new palettes were checked against their source files. Their content matches; only line endings differ between the repository checkout and the card.

## Device checklist

| Check | Result |
| --- | --- |
| Boots to the game-system list by default | Pass |
| Pills render with clean rounded corners and centered text | Pass |
| List typography is reduced and remains readable | Pass |
| Selection glide is smooth | Pass |
| Screen transitions are smooth | Pass |
| Six new palettes appear in Settings | Pass |
| Game launches and exits to the frontend | Pass |

The device checks were performed on the RG40XX H with the installed `12927c60` build.

## Reproducing the build

```sh
git clone https://github.com/NadimHider/nocturne.git
cd nocturne
git checkout 12927c60
make setup
make build PLATFORM=h700 COMPILE_CORES=false
```

The H700 workflow uses the same source and toolchain image. The build is not reproducible byte for byte for `minarch.elf` and `settings.elf`, so release artifacts should be taken from the workflow run that matches the release commit.

## Known limitations

- Nocturne is a component bundle. It does not install Base OS, a bootloader, a kernel, emulator cores, BIOS files, or ROMs.
- The release targets the H700 platform and the RG40XX H. Other H700 devices may work but are not part of this validation.
- Emulator performance and compatibility depend on the selected libretro core, ROM set, and BIOS files.
- Wi-Fi and Bluetooth behavior depends on the Base OS drivers and the local network or accessory.
- HDMI behavior depends on the display and cable. Hotplug handling restarts the frontend by design.

## Release artifact

`Nocturne-v1.0.1-H700-components.zip` contains the frontend, MinArch, key monitor, supporting utilities, ten palettes, Settings, LED Control, CleanRomset, and `.system/version.txt`.

SHA256:

```text
8EFD1C7D4FFA636DD0C6AE0F1B7FC8E422D4CFA43FC6108A844D9F67CB4F55B1
```

Verify the digest before extracting the archive to a card.
