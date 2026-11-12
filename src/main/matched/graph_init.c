/* Address: 0x80041000 */
/* Size: 172 bytes (43 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "common.h"

extern void GsInitGraph(s32 width, s32 height, s32 x, s32 y, s32 flag);
extern void GsSetDrawBuffOffset(void);
extern void GsDefDispBuff(s32 x, s32 y, s32 w, s32 h);
extern void GsInit3D(void);
extern void InitGeom(void);
extern void GsInitVcount(void);
extern s32 GsGetActiveBuff(void);
extern void draw_and_disp_env_init(void);

extern s32 ot_work;
extern void *ot_work0_org;
extern s32 ot_work1_len;
extern void *ot_work1_org;
extern u8 ot_buff0[1024];
extern u8 ot_buff1[1024];
extern s32 activeBuff;

void graph_init(void)
{

    GsInitGraph(320, 240, 0, 0, 0);
    GsSetDrawBuffOffset();

    GsDefDispBuff(0, 0, 0, 240);

    ot_work = 8;
    ot_work0_org = ot_buff0;
    ot_work1_len = 8;
    ot_work1_org = ot_buff1;

    GsInit3D();
    InitGeom();
    GsInitVcount();

    activeBuff = GsGetActiveBuff();

    draw_and_disp_env_init();
}
