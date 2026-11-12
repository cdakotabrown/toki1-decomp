/* Function: date_kanransya_text_set */
/* Overlay: DATE */
#include "types.h"

extern u8 ba_string2;
extern u8 date_kanransya_text_set_str0;

extern void sub_sub_step_up(void);
extern void strcpy(void *, void *);

void date_kanransya_text_set(void)
{
    strcpy((void *)&ba_string2, (void *)&date_kanransya_text_set_str0);
    sub_sub_step_up();
}
