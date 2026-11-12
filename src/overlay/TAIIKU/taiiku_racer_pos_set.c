/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1e0 --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x1ec --offset2 0x1f0 */
/* @PostProc: patch_insn_reg_field 0x1c:rt:a2:t1 0x20:rt:a1:a3 0x24:rt:a0:a2 0x28:rt:v1:a1 0x30:rt:s2:s1 0x34:rt:t4:s2 0x38:rt:v0:v1 0x40:rs:v1:a1 0x40:rt:v1:a1 0x44:rs:a0:a2 0x44:rt:a0:a2 0x48:rs:a1:a3 0x48:rt:a1:a3 0x4c:rs:a2:t1 0x4c:rt:a2:t1 0x54:rs:v0:v1 0x54:rt:v0:v1 0x58:rs:t4:s2 0x58:rt:t4:s2 0x5c:rs:s2:s1 0x5c:rt:s2:s1 0x60:rt:s1:s0 0x64:rd:a3:v0 0x68:rt:s0:t5 0x6c:rt:t5:t4 0x74:rs:v0:v1 0x84:rs:a3:v0 0x88:rt:a1:a3 0x90:rd:t1:a0 0x94:rs:t1:a0 0x98:rs:t4:s2 0xa0:rs:a3:v0 0xa4:rs:t1:a0 0xac:rs:v1:t4 0xac:rt:t5:a1 0xb4:rs:a0:a2 0xb4:rt:s0:t5 0xd0:rt:s1:s0 0xe4:rs:v0:v1 0xec:rt:s0:t5 0xf4:rs:t7:t2 0xf4:rt:t2:t7 0x104:rt:s1:s0 0x118:rs:v0:v1 0x120:rt:s0:t5 0x128:rs:t6:t2 0x128:rt:t2:t6 0x130:rs:v0:v1 0x138:rt:s1:s0 0x140:rs:t4:s2 0x150:rs:t1:a0 0x154:rs:t1:a0 0x158:rs:a3:v0 0x164:rs:v1:t4 0x164:rt:t5:a1 0x168:rt:a2:t1 0x16c:rs:s2:s1 0x178:rt:s1:s0 0x188:rs:t1:a0 0x18c:rs:v0:v1 0x194:rt:s0:t5 0x19c:rs:t9:t2 0x19c:rt:t2:t9 0x1ac:rt:s1:s0 0x1b8:rs:t1:a0 0x1bc:rs:a3:v0 0x1bc:rt:a3:v0 0x1c0:rs:a3:v0 0x1c8:rs:v0:v1 0x1c8:rt:v0:v1 0x1cc:rt:v1:v0 0x1d0:rt:v0:v1 0x1d8:rt:a1:t1 0x1dc:rs:v1:v0 0x1dc:rt:v1:v0 0x1e0:rs:a1:t1 0x1e0:rt:a1:t1 0x1e8:rs:v0:v1 0x1e8:rt:v0:v1 0x1f0:rt:t1:a3 0x1f4:rt:a3:a2 0x1f8:rs:v0:v1 0x200:rs:a3:a2 0x218:rs:v0:v1 0x220:rs:t1:a3 0x228:rs:v0:v1 0x230:rd:a2:a1 0x230:rt:v1:v0 0x234:rs:a2:a1 0x23c:rs:a2:a1 0x258:rt:a1:t1 0x25c:rs:v0:v1 0x25c:rt:v0:v1 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* Address: 0x80137D30 */
/* Size: 612 bytes (153 insns)   BYTE-PERFECT (name-derived resolved link) */
#include "types.h"
#include "tokimeki/taiiku_kari_run.h"
#include "tokimeki/class_w.h"

extern u8  class_w[];
extern u8  D_801271B0_b[];
extern u8  D_801493F8_b[];
extern s32 taiiku_konami_mode;
extern u8  taiiku_kari_gseat;
extern u8  taiiku_kari_girl;
extern u8  taiiku_kari_run[];
extern s16 taiiku_kari_scrl_hi;
extern s16 taiiku_kari_frm_col[];
extern s16 taiiku_kari_gl_card[];

void taiiku_racer_pos_set(void) {
    u8 *base;
    u8 *a0b;
    u8 *end;
    s32 state;
    u32 ga2;
    u32 ga3;
    u32 gt1;
    u8 *pv1;
    s16 *arrS2;
    s16 *arrS1;
    s32 i;
    s16 t0;
    s32 d;
    s16 refv;

    base = class_w;
    state = taiiku_konami_mode;
    ga2 = taiiku_kari_gseat;
    ga3 = ((TAIIKU_KARI_RUN *)taiiku_kari_run)[0].col;
    gt1 = taiiku_kari_girl;
    pv1 = taiiku_kari_run;
    arrS2 = taiiku_kari_frm_col;
    arrS1 = taiiku_kari_gl_card;

    for (i = 0; i < 4; i++) {
        if (pv1[0] != 0) {
            *(s16 *)(base + 7314 + i * 68) =
                *(s16 *)(base + 6566 + i * 68) - 8;
            if (i == 0) {
                t0 = arrS2[ga3];
                if (state == 1) {
                    ((CLASS_W *)class_w)[107].frame =
                        (*(s16 *)(base + 3150 + ga2 * 68) >> 3) * 6 + t0;
                } else {
                    ((CLASS_W *)class_w)[107].frame =
                        (*(s16 *)(base + 3150 + (u32)pv1[2] * 68) >> 3)
                        * 6 + t0;
                }
            } else {
                *(s16 *)(base + 7300 + i * 68) =
                    arrS2[pv1[3]]
                    + (*(s16 *)(base + 3150 + (u32)pv1[2] * 68) >> 3) * 6;
            }
            if ((i + 144) == *(s16 *)(base + 6552 + i * 68)) {
                if (state == 1) {
                    *(s16 *)(base + 7300 + i * 68) = arrS1[gt1] * 6 + 5;
                } else {
                    *(s16 *)(base + 7300 + i * 68) =
                        (*(s16 *)(base + 3150 + (u32)pv1[2] * 68) >> 3)
                        * 6 + 5;
                }
            }
        }
        pv1 += 36;
    }

    refv = taiiku_kari_scrl_hi;
    a0b = D_801271B0_b;
    end = D_801271B0_b + 204;
    pv1 = D_801493F8_b;
    do {
        if (pv1[41] == 2 && a0b[7346] != 0) {
            goto check70;
        }
        if (pv1[36] != 1U) {
            goto store0;
        }
    check70:
        d = *(s16 *)(pv1 + 70) - refv;
        if (d >= 320) {
            goto store0;
        }
        if (d < -319) {
            goto store0;
        }
        a0b[7347] = 164;
        goto next;
    store0:
        a0b[7347] = 0;
    next:
        a0b += 68;
        pv1 += 36;
    } while (a0b != end);
}
