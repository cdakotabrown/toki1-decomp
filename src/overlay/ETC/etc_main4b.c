/* Address: 0x8013ACB8 */
/* Size: 132 bytes (33 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
extern ETC_EV_RES_BG etc_ev4b_res;

extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0);
extern void bg_set6(s32 a0, s32 a1, s32 a2, s32 a3, s32 stack0, s32 stack1);
extern void date_object_set(s32 a0, s32 a1, s32 a2, s32 a3);


void etc_main4b(void)
{
    palette_upload_enqueue(etc_ev4b_res.pal, 17, 1, 2, 0);
    bg_set6(etc_ev4b_res.bg[0], etc_ev4b_res.bg[1], etc_ev4b_res.bg[2], etc_ev4b_res.bg[3],
                  etc_ev4b_res.bg[4], etc_ev4b_res.bg[5]);
    date_object_set(0, 0, 0, 0);
}
