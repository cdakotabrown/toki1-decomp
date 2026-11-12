/* Address: 0x80137620 */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

extern void palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void date_object_set(s32, s32, s32, s32);
extern void etc_ev0e_res_set(void);
extern void bg_set6(s32, s32, s32, s32, s32, s32);
extern s32 etc_ev0e_res_pal;
extern s32 etc_ev0e_res_bg0;
extern s32 etc_ev0e_res_bg1;
extern s32 etc_ev0e_res_bg2;
extern s32 etc_ev0e_res_bg3;
extern s32 etc_ev0e_res_bg4;
extern s32 etc_ev0e_res_bg5;

void etc_main0e(void)
{
    etc_ev0e_res_set();
    palette_upload_enqueue(etc_ev0e_res_pal, 17, 1, 2, 0);
    bg_set6(etc_ev0e_res_bg0, etc_ev0e_res_bg1, etc_ev0e_res_bg2, etc_ev0e_res_bg3, etc_ev0e_res_bg4, etc_ev0e_res_bg5);
    date_object_set(0, 0, 0, 0);
}
