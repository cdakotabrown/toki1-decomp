/* geko_g_name_set -- GEKO overlay, Address: 0x8013CAE4, Size: 60 bytes (15 insns) */
#include "types.h"

extern void strcpy(void *a0, void *a1);
extern void sub_sub_step_up(void);

extern u8 g_name;
extern u8 geko_g_name_set_str0;
extern s16 cal_cw_anim;

void geko_g_name_set(void)
{
    strcpy(&g_name, &geko_g_name_set_str0);
    cal_cw_anim = 5;
    sub_sub_step_up();
}
