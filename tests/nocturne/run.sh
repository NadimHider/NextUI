#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
mkdir -p tests/nocturne/build
cc -std=c11 -Wall -Wextra -Werror -Iworkspace/all/common tests/nocturne/test_theme.c -lm -o tests/nocturne/build/test_theme
cc -std=c11 -Wall -Wextra -Werror -Iworkspace/all/common tests/nocturne/test_switcher.c -o tests/nocturne/build/test_switcher
cc -std=c11 -Wall -Wextra -Werror -DUSE_SDL2 -DPLATFORM=\"desktop\" -Iworkspace/all/common -Iworkspace/desktop/platform $(pkg-config --cflags sdl2 SDL2_image) tests/nocturne/test_preview_cache.c $(pkg-config --libs sdl2 SDL2_image) -o tests/nocturne/build/test_preview_cache
(cd tests/nocturne/build && ./test_theme && ./test_switcher && ./test_preview_cache)
