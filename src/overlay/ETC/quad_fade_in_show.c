/* Address: 0x80132C3C (window absorbs the cross-prologue head that was */
/* Size: 884 bytes (221 insns) */
/* @SymbolStart: 0x80132C3C */
/* @SymbolSize: 884 */
/* @CompileFlags: -Wo,-varref,13 */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1:deny,lod:7:deny,lod:9:deny IDO52_FORCE_WEB_COLOR=130:a1,133:a3,131:t1,134:t2,135:t3 IDO52_AS1_TIE_TAKE=238 */
#include "types.h"

extern u8 *prim_alloc();
extern s32 GetTPage();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetPolyFT4();

extern s32 activeBuff;
extern u8 ot_buff0[];
extern s32 quad_fade_lv;
extern u8 step_mode;

s32 quad_fade_in_show(void)
{
    u8 pad[48];
    s32 slot;
    s32 yBase;
    s32 color;
    s32 cnt;
    u8 *s0;
    u8 *s1;
    u8 *s2;
    u8 *s3;

    yBase = -activeBuff * 240 + 240;
    s0 = prim_alloc(40, activeBuff);
    s1 = prim_alloc(40, activeBuff);
    s2 = prim_alloc(40, activeBuff);
    s3 = prim_alloc(40, activeBuff);

    SetPolyFT4(s0);
    SetPolyFT4(s1);
    SetPolyFT4(s2);
    SetPolyFT4(s3);

    SetSemiTrans(s0, 1);
    SetSemiTrans(s1, 1);
    SetSemiTrans(s2, 1);
    SetSemiTrans(s3, 1);

    color = quad_fade_lv;
    s0[4] = color;
    s0[5] = color;
    s0[6] = color;
    s1[6] = color;
    s1[5] = color;
    s1[4] = color;
    s2[6] = color;
    s2[5] = color;
    s2[4] = color;
    s3[6] = color;
    s3[5] = color;
    s3[4] = color;
    quad_fade_lv = color;

    *(s16 *)(s0 + 22) = GetTPage(2, 2, 640, 0);
    *(s16 *)(s1 + 22) = GetTPage(2, 2, 768, 0);
    *(s16 *)(s2 + 22) = GetTPage(2, 1, 320, 256);
    *(s16 *)(s3 + 22) = GetTPage(2, 1, 448, 256);

    *(s16 *)(s0 + 16) = 160;
    *(s16 *)(s0 + 32) = 160;
    *(s16 *)(s0 + 26) = (s16)(yBase + 179);
    *(s16 *)(s0 + 34) = (s16)(yBase + 179);
    *(s16 *)(s0 + 10) = (s16)(yBase + 19);
    *(s16 *)(s0 + 18) = (s16)(yBase + 19);
    *(u16 *)(s0 + 8) = 24;
    *(u16 *)(s0 + 24) = 24;
    s0[20] = 160;
    s0[36] = 160;
    s0[13] = 19;
    s0[21] = 19;
    s0[29] = 179;
    s0[37] = 179;
    s0[12] = 24;
    s0[28] = 24;

    *(s16 *)(s1 + 16) = 296;
    *(s16 *)(s1 + 32) = 296;
    *(s16 *)(s1 + 34) = (s16)(yBase + 179);
    *(s16 *)(s1 + 26) = (s16)(yBase + 179);
    *(s16 *)(s1 + 24) = 160;
    *(s16 *)(s1 + 18) = (s16)(yBase + 19);
    *(s16 *)(s1 + 10) = (s16)(yBase + 19);
    *(s16 *)(s1 + 8) = 160;
    s1[20] = 168;
    s1[36] = 168;
    s1[12] = 32;
    s1[28] = 32;
    s1[37] = 179;
    s1[29] = 179;
    s1[21] = 19;
    s1[13] = 19;

    *(s16 *)(s2 + 34) = (s16)(yBase + 179);
    *(s16 *)(s2 + 32) = 160;
    *(s16 *)(s2 + 26) = (s16)(yBase + 179);
    *(u16 *)(s2 + 24) = 24;
    *(s16 *)(s2 + 18) = (s16)(yBase + 19);
    *(s16 *)(s2 + 16) = 160;
    *(s16 *)(s2 + 10) = (s16)(yBase + 19);
    *(u16 *)(s2 + 8) = 24;
    s2[37] = 179;
    s2[36] = 160;
    s2[29] = 179;
    s2[28] = 24;
    s2[21] = 19;
    s2[20] = 160;
    s2[13] = 19;
    s2[12] = 24;

    *(s16 *)(s3 + 34) = (s16)(yBase + 179);
    *(s16 *)(s3 + 32) = 296;
    *(s16 *)(s3 + 26) = (s16)(yBase + 179);
    *(s16 *)(s3 + 24) = 160;
    *(s16 *)(s3 + 18) = (s16)(yBase + 19);
    *(s16 *)(s3 + 16) = 296;
    *(s16 *)(s3 + 10) = (s16)(yBase + 19);
    *(s16 *)(s3 + 8) = 160;
    s3[37] = 179;
    s3[36] = 168;
    s3[29] = 179;
    s3[28] = 32;
    s3[21] = 19;
    s3[20] = 168;
    s3[13] = 19;
    s3[12] = 32;

    AddPrim(&ot_buff0[(activeBuff << 10) + 20], s0);
    AddPrim(&ot_buff0[(activeBuff << 10) + 20], s1);
    AddPrim(&ot_buff0[(activeBuff << 10) + 4], s2);
    AddPrim(&ot_buff0[(activeBuff << 10) + 4], s3);

    cnt = quad_fade_lv + 2;
    quad_fade_lv = cnt;
    if (cnt >= 129) {
        step_mode += 1;
        cnt = 0;
        quad_fade_lv = cnt;
    }
}
