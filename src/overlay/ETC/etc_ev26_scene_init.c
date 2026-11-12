/* Address: 0x80138D6C */
/* Size: 140 bytes (35 instructions) */
#include "types.h"
#include "tokimeki/etc_ev_res.h"
extern ETC_EV_RES_BG etc_ev26_res;

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void etc_ev26_res_set(void);
extern void bg_set6(s32, s32, s32, s32, s32, s32);

void etc_ev26_scene_init(void)
{
    etc_ev26_res_set();
    palette_upload_enqueue(etc_ev26_res.pal, 17, 1, 2, 0);
    bg_set6(etc_ev26_res.bg[0], etc_ev26_res.bg[1], etc_ev26_res.bg[2], etc_ev26_res.bg[3], etc_ev26_res.bg[4], etc_ev26_res.bg[5]);
    date_object_set(0, 0, 0, 0);
}
