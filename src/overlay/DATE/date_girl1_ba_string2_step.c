/* Function: date_girl1_ba_string2_step */
/* Overlay: DATE */
#include "types.h"

extern u8 ba_string2;
extern u8 date_girl1_ba_string2_step_str0;

extern void sub_sub_step_up(void);
extern void strcpy(void *, void *);

void date_girl1_ba_string2_step(void)
{
    strcpy((void *)&ba_string2, (void *)&date_girl1_ba_string2_step_str0);
    sub_sub_step_up();
}
