#include <raylib.h>
#include "timer.h"

timer new_timer(float duration)
{
    return (timer){ .curr = 0, .duration = duration };
}

void finish_timer(timer *timer)
{
    timer->curr = timer->duration;
}

void reset_timer(timer *timer)
{
    timer->curr = 0;
}

int update_timer(timer *timer)
{
    timer->curr += GetFrameTime();

    if (timer->curr >= timer->duration) return 1;
    return 0;
}