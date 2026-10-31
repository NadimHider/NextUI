# Nocturne validation and audit notes

Date: 2026-10-31
Branch: `personal/nocturne`
Base fork HEAD: `440325896a59703e772f59fa7cf3d625124987a3`
Target: Anbernic RG40XX H (`h700`), Base OS + NextUI frontend

## 1. Pre-change audit summary

The personal fork was eight commits ahead of the upstream base used for this
work. The original fork work included:

- font size / LED fixes and retro font additions
- H700 packaging work
- curated fonts with Departure Mono as default
- release-asset ignore rules
- font hiding / MAME / shader cleanup
- START/MENU Settings behavior and in-game shortcuts
- CI trigger changes
- Menu-to-Settings fallback and per-button timing

Main issues found before editing:

- Select duplicated the game-switcher role and could be accidental.
- Several in-game shortcut defaults were aggressive for normal play.
- Light/white hardcoded colors remained across settings, overlays, clock,
  game time, LED control, and MiniArch surfaces.
- The switcher decoded previews repeatedly and had weak empty/unavailable
  states.
- H700 keymon consumed Select for color temperature and used a timing path
  that could mishandle idle/resume gaps.
- Desktop harness geometry did not match the RG40XX H panel by default.
- README and on-card readmes still described upstream control defaults.

## 2. Implemented scope

### Controls

- Menu tap opens Settings, with Quick Menu fallback.
- Start opens Quick Menu.
- Menu+R1 opens the Game Switcher from list, Quick Menu, and Settings.
- Select is unassigned at the frontend level on H700/desktop.
- H700 keymon no longer consumes Select; physical Menu+volume adjusts
  brightness. Color temperature remains in Settings > Display.
- MiniArch legacy shortcut defaults are migrated once, only when they exactly
  match the old defaults. The marker `nocturne_shortcuts = 1` is written with
  the active MinArch config. Custom nonlegacy bindings are preserved.
- Hardcoded Menu+Select exit was removed.

### Theme and motion

- Seven semantic color slots are preserved for compatibility:
  selection, header/footer surface, raised surface, primary text, selection
  text, secondary text, backdrop.
- Selection text is derived from the selection fill.
- Imported colors are normalized: dark opaque surfaces and lifted text
  contrast. Original palette files are not rewritten.
- Built-in palettes are dark-only: Nocturne, Ember, Iris, Tidal.
- Settings, keyboard prompt, Wi-Fi/Bluetooth, clock, game time, LED control,
  MiniArch, and frontend rows use semantic colors instead of white/black
  hardcodes where those were user-facing fills/text.
- Transitions are 100 ms snappy / 160 ms comfortable, with zero-duration
  guards. Title scrolling follows the animation setting.

### Switcher

- Three-entry main-thread LRU preview cache.
- Position counter and clearer title/footer hints.
- Repeat left/right navigation; wrap only when more than one recent exists.
- Two-step Y removal; B cancels. Removal affects only the recent list.
- Missing-preview and unavailable-recent fallbacks.
- Save/resume/persist paths and file formats were not changed.

### Device/build/docs

- Desktop harness defaults to 640x480, scale 2, six rows.
- `COMPILE_CORES=false` now actually skips cores.
- H700 keymon makefile no longer depends on the container mount path.
- H700 poweroff/reboot links libm after theme math became reachable.
- Personal package omits the upstream auto-updater pak.
- H700 CI runs host policy tests and builds a component artifact.
- Inherited upstream CI/release jobs are guarded against accidental personal
  fork execution; release jobs are upstream-only.
- README, NOTICE, CONTRIBUTING, and on-card readmes were updated for Nocturne.

## 3. Host policy tests

Command:

```bash
make test-nocturne
```

Result: PASS

- `theme: ok`
- `switcher: ok`
- `preview cache: ok`

The tests cover luminance/contrast normalization, selection-text derivation,
navigation/removal policy, and LRU promotion/eviction/clear behavior.

## 4. Native desktop build

Command:

```bash
make build PLATFORM=desktop COMPILE_CORES=false
```

Result: PASS

Built native desktop components include NextUI, MiniArch, clock, minput,
nextval, and Settings. The desktop harness uses `/var/tmp/nextui/sdcard` and
stub battery/network state. Remaining warnings are inherited upstream
warnings (for example `pclose` attribute and legacy string/format warnings).

## 5. H700 cross-builds

Docker/Podman were unavailable in the workspace. The official
`ghcr.io/loveretro/h700-toolchain:latest` image was downloaded with skopeo,
its `/opt` toolchain layers were extracted, and the cross compiler was used
directly with the image's sysroot and `/opt/nextui` support libraries.

Result: PASS for the changed/supporting H700 components:

- `libmsettings`
- `nextui`
- `minarch`
- `settings`
- `clock`
- `gametime` / `libgametimedb`
- `ledcontrol`
- `keymon`
- `battery`, `libbatmondb`, `batmon`
- `gametimectl`, `minput`, `syncsettings`, `nextval`
- `audiomon`, `show2`, `bootlogo`
- `poweroff_next` / `reboot_next`

`keymon.c` also passed a strict syntax check with upstream header
unused-parameter warnings suppressed.

Not run: full H700 package/core rebuild, `make h700`, and device flashing.
The H700 CI artifact is a component bundle, not a complete installer.

## 6. Screenshots

Captured from the native desktop harness under Xvfb at 640x480. They are
**desktop renders**, not RG40XX H device captures. The library entries are
placeholders.

Files:

- `docs/screenshots/desktop-main.png`
- `docs/screenshots/desktop-switcher.png`
- `docs/screenshots/desktop-switcher-2.png`
- `docs/screenshots/desktop-switcher-remove.png`
- `docs/screenshots/desktop-quick.png`
- `docs/screenshots/desktop-settings.png`
- `docs/screenshots/desktop-settings-appearance.png`

## 7. Physical RG40XX H checklist

Before daily use, verify on the actual device:

1. Boot Base OS + NextUI with the existing `MinUI.zip` / `.system/h700` layout.
2. Menu tap opens Settings; Start opens Quick Menu; Select does nothing in the
   frontend.
3. Menu+R1 opens the switcher from list, Quick Menu, and Settings; returning
   does not also trigger a Menu tap.
4. Volume keys adjust volume; physical Menu+volume adjusts brightness; no
   Select color-temperature chord remains.
5. Settings > Display color temperature still works.
6. Game Switcher previews load, cycling is responsive, Y removal requires two
   presses, B cancels, and ROM/save files are untouched.
7. Resume, quicksave, auto-resume, and `game_switcher.txt` persistence survive
   reboot and sleep/wake.
8. MiniArch menu, remaps, save/load state, reset, fast-forward, rewind, and
   achievement overlays still work; legacy shortcut migration does not
   overwrite custom bindings.
9. Dark palettes remain readable in low light and daylight; text contrast,
   button glyphs, keyboard, Wi-Fi/Bluetooth, clock, game time, and LED control
   have no white flashes or invisible text.
10. Animations feel immediate; disabling animations removes motion and
    scrolling without breaking rendering.
11. HDMI output, sleep/wake, audio, Wi-Fi/Bluetooth, battery reporting, LED
    banks, rumble, and thermal behavior remain stable.
12. Capture real device screenshots for the README.

## 8. Publication status

No commit push, release, deployment, or flashing was performed. Changes are
local to the workspace branch `personal/nocturne`.
