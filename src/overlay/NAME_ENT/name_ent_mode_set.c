/* @SymbolSize: 616 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_stack_complete --old 32 --new 48 */
/* @PostProc: scheduler_normalizer --target-bin assets/overlays/NAME_ENT.EXN --address 0x801395A0 --block-aware --reloc-symbol-fp */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* @PostProc: patch_insn_reg_field 0xc0:rd:v1:v0 0xcc:rs:v1:v0 0xd0:rs:v1:v0 0xfc:rs:t1:t3 0xfc:rt:t3:t1 0x104:rt:t4:t5 0x10c:rs:t4:t5 0x10c:rt:t6:t8 0x110:rt:t6:t8 0x114:rs:t6:t8 0x114:rt:t7:t9 0x118:rt:t7:t9 0x12c:rd:t8:t0 0x130:rs:t8:t0 0x134:rt:t9:t1 0x13c:rd:t0:t2 0x13c:rt:t9:t1 0x140:rs:v1:t2 0x140:rt:t0:v1 0x148:rt:t1:t4 0x150:rs:t1:t4 0x150:rt:t3:t7 0x154:rt:t3:t7 0x158:rs:t3:t7 0x158:rt:t4:t8 0x15c:rt:t4:t8 0x1c0:rt:a1:v1 0x1c4:rs:a1:v1 0x1c4:rt:a1:v1 0x1e0:rt:a1:v1 0x230:rt:t5:t9 0x23c:rt:a1:a0 0x240:rs:a1:a0 0x240:rt:a1:a0 0x248:rt:t5:t9 0x260:rt:a1:a0 */
/* name_ent_mode_set - NAME_ENT overlay, Address: 0x801395A0, Size: 616 bytes */
#include "types.h"

extern u8 name_ent_sei[];
extern u8 name_ent_mei[];
extern u8 name_ent_adana[];
extern u8 name_ent_mode_set_str0[];
extern u8 name_ent_mode_set_str1[];
extern u8 name_ent_mode_set_str2[];
extern u8 name_ent_mode_set_str3[];
extern u8 name_ent_mode_set_str4[];
extern u8 name_ent_mode_set_str5[];
extern u8 name_ent_mode_set_str6[];
extern u8 name_ent_mode_set_str7[];
extern u8 name_ent_mode_set_str8[];
extern u8 screen_w[];
extern s16 para_tai;
extern u8 girl0_basyo;

extern s32 strcmp(u8 *, u8 *);

void name_ent_mode_set(void)
{
    s32 i;
    u8 *q;
    u8 *ptr;
    u8 *end;
    u8 *dim;
    u32 modeHigh;

    if (strcmp(name_ent_adana, name_ent_mode_set_str0) == 0) {
        para_tai = 573;
        i = 1;
        q = screen_w + 4;
        do {
            i += 4;
            *(s16 *)(q + 258) = 573;
            *(s16 *)(q + 262) = 573;
            *(s16 *)(q + 266) = 573;
            q += 16;
            *(s16 *)(q + 238) = 573;
        } while (i != 9);
    }

    if (strcmp(name_ent_sei, name_ent_mode_set_str1) == 0 &&
        strcmp(name_ent_mei, name_ent_mode_set_str2) == 0) {
        dim = screen_w + i * 56;
        *(s16 *)(dim + 446) = 32;
        *(s16 *)(dim + 450) = 64;
        ptr = screen_w + 56;
        end = screen_w + 504;
        do {
            modeHigh = (u32)girl0_basyo >> 4;
            if ((u32)ptr[458] >> 4 != modeHigh) {
                *(s16 *)(ptr + 446) = 127;
                ptr[456] |= 0x02;
                ptr[456] |= 0x08;
                *(s16 *)(ptr + 450) = 127;
                modeHigh = (u32)girl0_basyo >> 4;
            }
            if ((u32)ptr[514] >> 4 != modeHigh) {
                *(s16 *)(ptr + 502) = 127;
                ptr[512] |= 0x02;
                ptr[512] |= 0x08;
                *(s16 *)(ptr + 506) = 127;
            }
            ptr += 112;
        } while (ptr != end);
    }

    if (strcmp(name_ent_sei, name_ent_mode_set_str3) == 0 &&
        strcmp(name_ent_mei, name_ent_mode_set_str4) == 0 &&
        strcmp(name_ent_adana, name_ent_mode_set_str5) == 0) {
        para_tai = 0;
        q = screen_w + 4;
        end = screen_w + 36;
        do {
            q += 16;
            *(s16 *)(q + 242) = 0;
            *(s16 *)(q + 246) = 0;
            *(s16 *)(q + 250) = 0;
            *(s16 *)(q + 238) = 0;
        } while (q != end);
    }

    if (strcmp(name_ent_sei, name_ent_mode_set_str6) == 0 &&
        strcmp(name_ent_mei, name_ent_mode_set_str7) == 0 &&
        strcmp(name_ent_adana, name_ent_mode_set_str8) == 0) {
        para_tai = 999;
        q = screen_w + 4;
        end = screen_w + 36;
        do {
            q += 16;
            *(s16 *)(q + 242) = 999;
            *(s16 *)(q + 246) = 999;
            *(s16 *)(q + 250) = 999;
            *(s16 *)(q + 238) = 999;
        } while (q != end);
    }
}
