/* @SymbolSize: 544 */
/* @CompileFlags: -Wc,-pic1 -Wo,-loopunroll,0 */
/* Address: 0x8013C1FC, Size: 544 bytes (136 instructions) */
/* @PostProc: patch_insn_reg_field 0xbc:rt:v1:v0 0xc4:rs:v1:v0 0xd0:rs:v1:v0 0xe4:rs:v1:v0 0xe8:rs:v1:v0 0xf4:rs:v1:v0 0x100:rs:v1:v0 0x108:rs:v1:v0 0x10c:rs:v1:v0 0x130:rs:v1:v0 0x140:rs:v1:v0 0x14c:rs:v1:v0 0x154:rs:v1:v0 0x15c:rs:v1:v0 0x16c:rs:t8:s2 0x16c:rt:s2:t8 0x184:rt:t5:t6 0x194:rt:t6:t7 0x1bc:rt:v1:t5 0x1cc:rt:t5:t6 0x1d0:rt:t6:t7 0x1d8:rt:v1:t5 0x1dc:rt:t7:t8 0x1e4:rd:t8:t9 0x1e4:rt:t7:t8 0x1e8:rd:t9:t0 0x1e8:rs:t8:s2 0x1e8:rt:s2:t9 0x1f0:rs:t9:t0 */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} PTag;

extern u8 *tt_disp_wk;
extern u8 *tt_bell_buf;
extern u8 *tt_scale_buf;
extern u8 *tt_db_buf;
extern void tt_sprite_ot_add_16x16(PTag *, void *, s32);
extern void tt_sprite_ot_add_var(PTag *, void *, s32);

void tt_sprite_pool_disp(void)
{
    u8 *heap;
    u8 *shared;
    u8 *rec;
    u8 *p;
    u8 *pb;
    PTag *ot;
    s32 i;
    s16 pair;
    u8 v13;

    heap = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    shared = tt_scale_buf;
    rec = tt_bell_buf;
    i = 0;
    do {
        if (rec[0] != 0 && (*(u16 *)(rec + 80) & 1) == 0) {
            p = *(u8 **)(heap + 4);
            pb = *(u8 **)(heap + 4);
            *(u8 **)(heap + 4) = p + 48;
            *(s16 *)(shared + 4) = *(s16 *)(rec + 20);
            *(s16 *)(shared + 6) = *(s16 *)(rec + 22);
            p[41] = 0;
            p[42] = 1;
            p[40] = rec[60];
            *(s16 *)(p + 22) = *(u16 *)(rec + 82);
            *(s16 *)(p + 14) = *(u16 *)(rec + 84);
            p[44] = 0;
            if (*(u16 *)(rec + 4) < 144U) {
                *(s16 *)(p + 8) = (s16)(*(s16 *)(rec + 20) - 8);
                *(s16 *)(p + 10) = (s16)(*(s16 *)(rec + 22) - 8);
                p[12] = rec[86];
                p[13] = (u8)(rec[87] + 128);
                p[43] = 0;
                ot = (PTag *)((heap + *(u16 *)(rec + 16) * 4) + 0x8008);
                tt_sprite_ot_add_16x16(ot, pb, (s32)shared);
            } else {
                pair = *(s16 *)(rec + 98);
                *(s16 *)(shared + 2) = pair;
                *(s16 *)(shared + 0) = pair;
                *(s16 *)(pb + 8) = (s16)(*(s16 *)(rec + 20) - 31);
                *(s16 *)(pb + 10) = (s16)(*(s16 *)(rec + 22) - 7);
                pb[12] = rec[86];
                v13 = rec[87];
                pb[43] = 1;
                *(s16 *)(pb + 16) = 64;
                *(s16 *)(pb + 18) = 16;
                *(s16 *)(pb + 22) = 11;
                *(s16 *)(pb + 14) = 15489;
                pb[40] = 0;
                pb[13] = v13;
                ot = (PTag *)((heap + *(u16 *)(rec + 16) * 4) + 0x8008);
                tt_sprite_ot_add_var(ot, pb, (s32)shared);
            }
        }
        i++;
        rec += 108;
    } while (i != 32);
}
