#ifndef NOCTURNE_SWITCHER_H
#define NOCTURNE_SWITCHER_H

/* Pure navigation policy, shared with the host test suite. Removal affects
 * the recent list only; ROMs, battery saves and state files are never touched. */
static inline int Switcher_move(int selected, int count, int delta)
{
    if (count <= 0) return 0;
    if (selected < 0 || selected >= count) selected = 0;
    if (count == 1) return 0;
    int next = (selected + delta) % count;
    return next < 0 ? next + count : next;
}

static inline int Switcher_afterRemove(int selected, int count)
{
    if (count <= 0) return 0;
    return selected >= count ? count - 1 : selected;
}

typedef enum {
    SWITCHER_REMOVE_IDLE = 0,
    SWITCHER_REMOVE_ARMED,
    SWITCHER_REMOVE_CONFIRMED
} SwitcherRemoveState;

static inline SwitcherRemoveState Switcher_requestRemove(SwitcherRemoveState state)
{
    return state == SWITCHER_REMOVE_ARMED ? SWITCHER_REMOVE_CONFIRMED : SWITCHER_REMOVE_ARMED;
}

#endif
