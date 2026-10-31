#include <assert.h>
#include <stdio.h>
#include "../../workspace/all/common/switcher.h"

int main(void)
{
    assert(Switcher_move(0, 0, 1) == 0);
    assert(Switcher_move(4, 1, -1) == 0);
    assert(Switcher_move(2, 3, 1) == 0);
    assert(Switcher_move(0, 3, -1) == 2);
    assert(Switcher_move(1, 3, 5) == 0);
    assert(Switcher_move(1, 3, -5) == 2);
    assert(Switcher_move(99, 3, 1) == 1);

    assert(Switcher_afterRemove(0, 0) == 0);
    assert(Switcher_afterRemove(2, 2) == 1);
    assert(Switcher_afterRemove(0, 2) == 0);

    SwitcherRemoveState state = SWITCHER_REMOVE_IDLE;
    state = Switcher_requestRemove(state);
    assert(state == SWITCHER_REMOVE_ARMED);
    state = Switcher_requestRemove(state);
    assert(state == SWITCHER_REMOVE_CONFIRMED);

    puts("switcher: ok");
    return 0;
}
