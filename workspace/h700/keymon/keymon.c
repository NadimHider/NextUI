#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <poll.h>
#include <time.h>
#include <linux/input.h>
#include <signal.h>

#include <msettings.h>

#define VOLUME_MIN 0
#define VOLUME_MAX 20
#define BRIGHTNESS_MIN 0
#define BRIGHTNESS_MAX 10

#define CODE_MENU 312
#define CODE_PLUS 115
#define CODE_MINUS 114
#define INPUT_MAX 32
#define REPEAT_DELAY_MS 300
#define REPEAT_RATE_MS 100

static volatile sig_atomic_t quit = 0;
static void on_term(int sig) { (void)sig; quit = 1; }

static uint32_t now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)ts.tv_sec * 1000U + (uint32_t)(ts.tv_nsec / 1000000L);
}

static void adjust(int delta, int menu_pressed)
{
    if (menu_pressed) {
        int value = (int)GetBrightness() + delta;
        if (value < BRIGHTNESS_MIN) value = BRIGHTNESS_MIN;
        if (value > BRIGHTNESS_MAX) value = BRIGHTNESS_MAX;
        SetBrightness(value);
    } else {
        int value = (int)GetVolume() + delta;
        if (value < VOLUME_MIN) value = VOLUME_MIN;
        if (value > VOLUME_MAX) value = VOLUME_MAX;
        SetVolume(value);
    }
}

int main(void)
{
    struct sigaction sa = {0};
    sa.sa_handler = on_term;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    InitSettings();

    struct pollfd fds[INPUT_MAX];
    int count = 0;
    DIR *dir = opendir("/dev/input");
    if (dir) {
        struct dirent *ent;
        while ((ent = readdir(dir)) && count < INPUT_MAX) {
            if (strncmp(ent->d_name, "event", 5) != 0) continue;
            char path[64];
            snprintf(path, sizeof(path), "/dev/input/%s", ent->d_name);
            int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
            if (fd < 0) continue;
            fds[count].fd = fd;
            fds[count].events = POLLIN;
            fds[count++].revents = 0;
        }
        closedir(dir);
    }

    uint32_t last = now_ms();
    uint32_t up_repeat_at = 0, down_repeat_at = 0;
    int menu_pressed = 0, up_pressed = 0, down_pressed = 0;
    int up_just = 0, down_just = 0;

    while (!quit) {
        uint32_t now = now_ms();
        int timeout = -1;
        if (up_pressed) timeout = (int)(up_repeat_at - now);
        if (down_pressed) {
            int down_timeout = (int)(down_repeat_at - now);
            if (timeout < 0 || down_timeout < timeout) timeout = down_timeout;
        }
        if (timeout < 0 && (up_pressed || down_pressed)) timeout = 0;
        int ready = poll(fds, count, timeout);
        if (ready < 0 && errno != EINTR) break;
        now = now_ms();
        /* A large monotonic gap means we were suspended. Clear held state, but
         * still accept the fresh events that woke us instead of dropping the
         * first button press after a long idle. */
        if (now - last > 1000) {
            menu_pressed = up_pressed = down_pressed = up_just = down_just = 0;
            up_repeat_at = down_repeat_at = 0;
        }

        for (int i = 0; i < count; i++) {
            if (!(fds[i].revents & POLLIN)) continue;
            struct input_event ev;
            while (read(fds[i].fd, &ev, sizeof(ev)) == (ssize_t)sizeof(ev)) {
                if (ev.type != EV_KEY || ev.value > 2) continue;
                switch (ev.code) {
                case CODE_MENU:
                    /* Physical MENU only. The synthetic KEY_GOTO pulse is not
                     * a hold and must never divert volume to brightness. */
                    menu_pressed = ev.value != 0;
                    break;
                case CODE_PLUS:
                    up_just = ev.value && !up_pressed;
                    up_pressed = ev.value != 0;
                    if (up_just) up_repeat_at = now + REPEAT_DELAY_MS;
                    break;
                case CODE_MINUS:
                    down_just = ev.value && !down_pressed;
                    down_pressed = ev.value != 0;
                    if (down_just) down_repeat_at = now + REPEAT_DELAY_MS;
                    break;
                default:
                    /* Select is intentionally not consumed here. It reaches
                     * games normally and has no standalone frontend action. */
                    break;
                }
            }
        }

        if (up_just || (up_pressed && (int32_t)(now - up_repeat_at) >= 0)) {
            adjust(1, menu_pressed);
            up_just = 0;
            up_repeat_at = now + REPEAT_RATE_MS;
        }
        if (down_just || (down_pressed && (int32_t)(now - down_repeat_at) >= 0)) {
            adjust(-1, menu_pressed);
            down_just = 0;
            down_repeat_at = now + REPEAT_RATE_MS;
        }
        last = now;
    }

    for (int i = 0; i < count; i++) close(fds[i].fd);
    QuitSettings();
    return 0;
}
