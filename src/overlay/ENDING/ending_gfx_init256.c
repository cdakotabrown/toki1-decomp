/* ending_gfx_init256 - ENDING overlay, Address: 0x80134150, Size: 152 bytes (38 instructions) */
#include "types.h"

extern void GsInitGraph();
extern void GsSetDrawBuffOffset();
extern void GsDefDispBuff();
extern void GsInit3D(void);
extern void InitGeom();

extern s32 ot_work;
extern s32 ot_work0_org;
extern s32 ot_work1_len;
extern s32 ot_work1_org;
extern s32 ot_buff0;
extern s32 ot_buff1;
extern s16 screen_w;

void ending_gfx_init256(void)
{
    GsInitGraph(256, 240, 0, 0, 0);
    GsSetDrawBuffOffset();
    GsDefDispBuff(0, 0, 0, 240);

    ot_work = 8;
    ot_work0_org = (s32)&ot_buff0;
    ot_work1_len = 8;
    ot_work1_org = (s32)&ot_buff1;
    GsInit3D();
    InitGeom();
    screen_w = 256;
}
