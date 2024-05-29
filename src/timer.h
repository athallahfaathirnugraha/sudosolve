#ifndef _TIMER_H_
#define _TIMER_H_

typedef struct timer
{
    float duration;
    float curr;
} timer;

timer new_timer(float duration);
void finish_timer(timer *timer);
void reset_timer(timer *timer);
// 1 = timer finished
int update_timer(timer *timer);

#endif