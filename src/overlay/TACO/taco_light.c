/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: patch_insn_reg_field 0x44:rt:v0:a2 0x50:rt:t7:a3 0x58:rs:v0:v1 0x58:rt:v0:v1 0x5c:rs:v1:a2 0x5c:rt:v1:a2 0x60:rs:t7:a3 0x60:rt:t7:a3 0x68:rs:v0:v1 0x68:rt:v0:t7 0x6c:rt:v0:t7 0x74:rt:v0:t7 0x7c:rs:v1:a2 0x7c:rt:v1:t8 0x80:rt:v1:t8 0x88:rt:v1:t8 0x90:rs:t7:a3 0x90:rt:t8:v0 0x94:rt:t8:v0 0x9c:rt:t8:v0 0xa4:rt:v0:t7 0xac:rt:v1:t8 0xb4:rt:t9:a1 0xb8:rs:t9:a1 0xbc:rt:t8:v0 0xc8:rt:t1:a1 0xcc:rs:t1:a1 0xd8:rt:t3:a1 0xdc:rs:t3:a1 0xfc:rt:t4:t0 0x100:rs:t4:t0 0x100:rt:t4:t0 0x108:rs:t4:t0 0x108:rt:t5:t1 0x110:rt:t5:t1 0x160:rt:t7:t3 0x1b0:rt:t7:t3 0x200:rt:t7:t3 0x250:rt:t7:t3 0x2a0:rt:t7:t3 0x2f0:rt:t7:t3 0x340:rt:t7:t3 0x34c:rt:t7:t3 0x358:rt:t7:t3 0x360:rt:t7:t3 */
/* @SymbolSize: 884 */
/* @CompileEnv: IDO52_UGEN_NO_ARG_COALESCE=1 IDO52_FORCE_WEB_COLOR=68:s1,71:s2 */
/* Address: 0x80134A28 */
/* Size: 884 bytes (221 insns) */
/* $t8).  @SymbolSize: 884 windows it here; the real fix is to shrink the .sym */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
    u8 d;
    u8 e;
    u8 f;
    u8 g;
} Cfg80146;

extern Cfg80146 pslt[];
extern s16 taco_light_cnt;
extern s32 taco_model_pose_tbl[];
extern u8 back_clear_b;

extern void GsSetFlatLight();
extern void taco_pose_keyframe_set();

s32 taco_light(void)
{
    s32 v;
    s32 t;
    s32 i;
    s32 n;
    s32 *p;
    s32 x0;
    s32 x1;
    s32 x2;

    v = taco_light_cnt;
    t = v;
    if (v < 32) {
        if (pslt[0].d < 192U) {
            x0 = pslt[0].f + 6;
            x1 = pslt[1].f + 1;
            x2 = pslt[2].f + 1;
            pslt[0].e = (u8)x0;
            pslt[0].d = (u8)x0;
            pslt[1].e = (u8)x1;
            pslt[1].d = (u8)x1;
            pslt[2].e = (u8)x2;
            pslt[2].d = (u8)x2;
            pslt[0].f = (u8)x0;
            pslt[1].f = (u8)x1;
            pslt[2].f = (u8)x2;
            GsSetFlatLight(0, &pslt[0]);
            GsSetFlatLight(1, &pslt[1]);
            GsSetFlatLight(2, &pslt[2]);
        }
        if (t < 15) {
            back_clear_b = back_clear_b + 1;
        }
    } else if (v >= 32 && v < 84) {
        i = 0;
        p = taco_model_pose_tbl;
        do {
            n = i + 1;
            taco_pose_keyframe_set(n, *p, 1);
            i = n;
            p++;
        } while (n != 2);
    } else if (v >= 84 && v < 104) {
        i = 0;
        p = taco_model_pose_tbl;
        do {
            n = i + 1;
            taco_pose_keyframe_set(n, *p, 2);
            i = n;
            p++;
        } while (n != 2);
    } else if (v >= 104 && v < 114) {
        i = 0;
        p = taco_model_pose_tbl;
        do {
            n = i + 1;
            taco_pose_keyframe_set(n, *p, 3);
            i = n;
            p++;
        } while (n != 2);
    } else if (v >= 114 && v < 124) {
        i = 0;
        p = taco_model_pose_tbl;
        do {
            n = i + 1;
            taco_pose_keyframe_set(n, *p, 4);
            i = n;
            p++;
        } while (n != 2);
    } else if (v >= 124 && v < 154) {
        i = 0;
        p = taco_model_pose_tbl;
        do {
            n = i + 1;
            taco_pose_keyframe_set(n, *p, 5);
            i = n;
            p++;
        } while (n != 2);
    } else if (v >= 154 && v < 174) {
        i = 0;
        p = taco_model_pose_tbl;
        do {
            n = i + 1;
            taco_pose_keyframe_set(n, *p, 6);
            i = n;
            p++;
        } while (n != 2);
    } else if (v >= 174 && v < 234) {
        i = 0;
        p = taco_model_pose_tbl;
        do {
            n = i + 1;
            taco_pose_keyframe_set(n, *p, 7);
            i = n;
            p++;
        } while (n != 2);
    } else if (v >= 240) {
        return 1;
    }
    taco_light_cnt = (s16)(taco_light_cnt + 1);
    return 0;
}
