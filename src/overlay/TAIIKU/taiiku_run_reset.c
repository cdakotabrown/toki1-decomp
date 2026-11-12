/* @SymbolSize: 96 */
/* Address: 0x8013D49C */
/* Size: 96 bytes (24 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void device_cmd_send(s32);

extern s32 taiiku_tama_pfrm;
extern s32 taiiku_tama_gfrm;
extern s16 taiiku_tama_ghold;
extern s32 taiiku_tama_vel;
extern s32 taiiku_tama_pstep;
extern s32 taiiku_tama_gstep;
extern CLASS_W class_w[];

void taiiku_run_reset(void)
{
    device_cmd_send(1284);
    taiiku_tama_pfrm = 0;
    taiiku_tama_gfrm = 0;
    taiiku_tama_ghold = 0;
    taiiku_tama_vel = 0;
    taiiku_tama_pstep = 0;
    taiiku_tama_gstep = 0;
    class_w[101].disp_flag = class_w[101].disp_flag | 0x80;
}
