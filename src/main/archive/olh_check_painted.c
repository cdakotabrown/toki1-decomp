/* Function: olh_check */
/* Address: 0x800679A0 */
/* Size: 952 bytes (238 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_olh_check.py */
#include "types.h"

extern void mascot_init(s32);
extern s32 rsin(s32);
extern s32 SquareRoot0(s32);
extern void line_set(s16, s16, s16, s16, s32, s32, s32);

extern s16 D_800C0718;
extern u32 D_800EF6F8;
extern u32 D_800EF898;
extern u32 D_800EF780;
extern u8 mascot_hi;
extern u8 mascot_cw;
extern s16 D_8012DCA0;
extern s16 D_8012DCB6;
extern s32 D_801278BC;
extern s16 D_801278B4;
extern s16 D_801278B8;
extern s16 D_801278C2;
extern s16 D_801278C6;
extern s16 D_801278F6;
extern s16 D_801278FA;

void olh_check(void) {
    s16 sp3A;
    s32 t0;
    s32 t1;
    s32 t2;
    u32 v0;
    s32 a2;
    u32 a1;
    s32 a0_h;

    if (D_800C0718 == 0 || D_800C0718 >= 14) {
        mascot_init(0);
        return;
    }

    mascot_init(D_800C0718);

    if (D_800C0718 >= 14) {
        return;
    }

    if ((D_800EF6F8 >> 16) != 0) {
        mascot_cw = (u8)((u32)(mascot_hi + 1) >> 1);
    }

    v0 = D_800EF898;
    if ((v0 & 0x3F) == 63) {
        if (mascot_cw != 0) {
            mascot_cw = mascot_cw - 1;
        }
    }

    v0 = rsin((v0 & 0x7F) << 5);

    sp3A = (s16)((v0 * (u32)mascot_cw) >> 12);
    D_801278BC = ((-(s16)sp3A * 2) / 3) << 12;

    v0 = SquareRoot0((s32)mascot_hi * (s32)mascot_hi - (s16)sp3A * (s16)sp3A);

    t0 = D_8012DCA0 + sp3A;
    a2 = mascot_hi;
    a0_h = D_8012DCB6;
    t1 = a0_h + a2;
    t2 = 15 - v0;
    D_801278C2 = (s16)t0;
    D_801278C6 = (s16)(t1 - t2);
    D_801278B4 = D_800C0718 - 1;
    a1 = D_800EF780;
    D_801278F6 = D_8012DCA0;
    D_801278FA = (s16)a0_h;

    if (a1 & 2) {
        D_801278B8 = 14;

        switch (a1 & 0x5000) {
        case 0x1000:
            if ((u32)a2 >= 3U) {
                a2 = (a2 - 2) & 0xFF;
                mascot_cw = (u8)((u32)(a2 + 1) >> 1);
                mascot_hi = (u8)a2;
                t1 = a0_h + a2;
            }
            break;
        case 0x4000:
            if ((u32)a2 < 196U) {
                a2 = (a2 + 2) & 0xFF;
                mascot_cw = (u8)((u32)(a2 + 1) >> 1);
                mascot_hi = (u8)a2;
                t1 = a0_h + a2;
            }
            break;
        }
    } else if (a1 & 8) {
        D_801278B8 = 14;
        v0 = a1 & 0x5000;
        if (v0 == 0x1000) goto case2_1000;
        if (v0 == 0x4000) goto case2_4000;
        goto case2_a000;

    case2_1000:
        D_8012DCB6 = D_8012DCB6 - 4;
        t1 = (s16)D_8012DCB6 + a2;
        goto case2_a000;

    case2_4000:
        D_8012DCB6 = D_8012DCB6 + 4;
        t1 = (s16)D_8012DCB6 + a2;

    case2_a000:
        v0 = a1 & 0xA000;
        if (v0 == 0x2000) goto case3_2000;
        if (v0 == (u32)0x8000) goto case3_8000;
        goto label_end;

    case3_2000:
        D_8012DCA0 = D_8012DCA0 + 4;
        t0 += 4;
        goto label_end;

    case3_8000:
        D_8012DCA0 = D_8012DCA0 - 4;
        t0 -= 4;
    } else {
        D_801278B8 = 13;
    }

label_end:
    v0 = 0xF800B0C8;
    line_set(D_8012DCA0, D_8012DCB6, (s16)t0, (s16)(t1 - t2),
                  (s32)v0, (s32)v0, 1);

    if (D_8012DCA0 >= 513) {
        D_8012DCA0 = -512;
    }
    if (D_8012DCA0 < -512) {
        D_8012DCA0 = 512;
    }
    if (D_8012DCB6 >= 513) {
        D_8012DCB6 = -512;
    }
    if (D_8012DCB6 < -512) {
        D_8012DCB6 = 512;
    }
}
