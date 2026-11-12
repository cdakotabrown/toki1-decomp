/* @SymbolSize: 732 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x21c */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x190 */
/* @PostProc: patch_insn_reg_field 0x58:rt:t5:a0 0x60:rd:a0:a1 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x74:rd:t1:t2 0x78:rd:a3:a2 0x78:rs:t1:t2 0x78:rt:t2:t3 0x8c:rs:a3:a2 0x90:rt:a2:a3 0xa0:rt:t2:t4 0xa4:rt:t3:t4 0xa8:rt:t5:t6 0xac:rs:t3:t4 0xac:rt:t4:t5 0xb0:rs:t5:t6 0xb0:rt:t6:t7 0xb4:rt:t4:t5 0xb8:rt:a2:a3 0xc0:rt:t6:t7 0xc4:rt:t7:t8 0xc8:rt:t9:t2 0xcc:rs:t2:t4 0xcc:rt:t3:t5 0xd0:rs:t7:t8 0xd0:rt:t8:t9 0xd4:rs:t9:t2 0xd4:rt:t1:t3 0xd8:rt:t8:t9 0xdc:rt:t1:t3 0xe0:rt:t3:t5 0x104:rs:a3:a2 0x114:rt:t3:t6 0x118:rt:t4:t6 0x11c:rt:t6:t8 0x120:rs:t4:t6 0x120:rt:t5:t7 0x124:rs:t6:t8 0x124:rt:t7:t9 0x128:rt:t5:t7 0x12c:rt:a2:a3 0x134:rt:t7:t9 0x138:rt:t8:t2 0x13c:rt:t1:t4 0x140:rs:t3:t6 0x140:rt:t4:t7 0x144:rs:t8:t2 0x144:rt:t9:t3 0x148:rs:t1:t4 0x148:rt:t2:t5 0x14c:rt:t9:t3 0x150:rt:t2:t5 0x154:rt:t4:t7 0x16c:rs:t5:a0 0x16c:rt:t5:a0 0x170:rt:t7:t9 0x174:rd:t6:t8 0x174:rt:t5:a0 0x178:rd:t6:t8 0x178:rs:t6:t8 0x178:rt:t5:a0 0x17c:rd:t6:t8 0x17c:rt:t6:t8 0x180:rs:t7:t9 0x180:rt:t7:t9 0x184:rd:a3:t0 0x184:rs:t6:t8 0x184:rt:t7:t9 0x190:rd:a1:a2 0x190:rs:a3:t0 0x194:rt:a2:a3 0x198:rs:a1:a2 0x198:rt:v1:t2 0x19c:rs:a1:a2 0x19c:rt:a1:a2 0x1a0:rd:t8:t4 0x1a0:rt:v1:t2 0x1a4:rs:t8:t4 0x1a8:rd:t9:t5 0x1a8:rs:a0:t0 0x1a8:rt:a3:a1 0x1ac:rs:t9:t5 0x1bc:rt:t1:t6 0x1c0:rt:t3:t8 0x1c4:rs:t1:t6 0x1c4:rt:t2:t7 0x1c8:rs:t3:t8 0x1c8:rt:t4:t9 0x1cc:rt:t2:t7 0x1d0:rt:a2:a3 0x1d8:rt:t4:t9 0x1dc:rt:t5:t2 0x1e0:rt:t7:t4 0x1e4:rs:t5:t2 0x1e4:rt:t6:t3 0x1e8:rs:t7:t4 0x1e8:rt:t8:t5 0x1ec:rt:t6:t3 0x1f4:rt:t8:t5 0x1f8:rt:t9:t6 0x1fc:rs:t9:t6 0x1fc:rt:t1:t7 0x200:rt:t1:t7 0x204:rs:a0:a1 0x204:rt:a0:a1 0x208:rs:a0:a1 0x21c:rt:a0:a1 0x220:rs:a3:t0 0x220:rt:a1:a2 0x224:rt:t0:t1 0x228:rs:a1:a2 0x228:rt:v1:t8 0x22c:rs:a1:a2 0x22c:rt:a1:a2 0x230:rt:v1:t8 0x238:rs:a0:t0 0x238:rt:a3:a1 0x260:rt:a2:a3 0x270:rt:t1:t2 0x278:rs:t1:t2 0x278:rt:t2:t3 0x284:rt:t2:t3 0x288:rt:t3:t4 0x28c:rs:t3:t4 0x28c:rt:t4:t5 0x290:rt:t4:t5 0x294:rs:a0:a1 0x294:rt:a0:a1 0x298:rs:a0:a1 0x298:rt:t0:t1 0x2a0:rt:t5:t6 0x2a4:rs:t5:t6 0x2a4:rt:t5:t6 0x2ac:rs:t5:t6 0x2ac:rt:t6:t7 0x2b0:rt:t6:t7 0x2b8:rt:t7:t8 0x2bc:rt:t7:t8 0x2c0:rt:t8:t9 0x2c4:rs:t8:t9 0x2c4:rt:t8:t9 0x2cc:rs:t8:t9 0x2cc:rt:t9:t2 0x2d4:rt:t9:t2 0x2dc:rt:t1:t3 0x2e0:rt:t1:t3 */
/* Address: 0x8013AB58 */
/* Size: 732 bytes (183 insns) */
#include "types.h"

extern u8 class_w[];
extern u8 D_801271B0_a[];
extern u8 D_801271B0_b[];
extern u8 cw8_rec[];
extern u8 D_801273D0_a[];
extern u8 D_801273D0_b[];
extern u8 cw11_active[];
extern u8 cw13_active[];
extern u8 card_sys_present;
extern u8 last_album;
extern u8 album_work[];
extern u8 album_slot;
extern u8 album_chr[];
extern volatile u8 cw136_prio;
extern volatile u8 cw136_disp;
extern volatile u8 cw140_prio;
extern volatile u8 cw140_disp;

extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);

void name_attr_set(void)
{
    u8 *p;
    u8 *q;
    u8 *s;
    u8 *base;
    u8 *rec;
    u8 *ent;
    s32 i;
    s32 c;
    s32 flags;

    k_sub_reset_point_set();
    k_sub_disp_start(0);

    p = class_w;
    do {
        p[8707] |= 0x80;
        p += 68;
    } while (p < cw13_active);

    if (card_sys_present != 0) {
        base = s = album_work + last_album * 16;
        q = D_801271B0_a;
        do {
            c = s[3272];
            if (c == 0) {
                q[8707] |= 0x80;
                q[8705] = 4;
                q[9591] &= 0x7F;
            } else {
                q[8707] &= 0x7F;
                q[9591] |= 0x80;
                *(s16 *)(q + 9612) = (c & 7) + 88;
            }
            q += 68;
            s++;
        } while (q < cw8_rec);

        s = base + 8;
        q = D_801273D0_a;
        do {
            c = s[3272];
            if (c == 0) {
                q[8775] |= 0x80;
                q[8773] = 4;
                q[9591] &= 0x7F;
            } else {
                q[8775] &= 0x7F;
                q[9591] |= 0x80;
                *(s16 *)(q + 9612) = (c & 7) + 88;
            }
            q += 68;
            s++;
        } while (q != cw11_active);
    } else {
        rec = ent = album_chr + album_slot * 3840;
        q = D_801271B0_b;
        for (i = 0; i < 8; i++) {
            flags = *(s32 *)(ent + 788);
            ent += 56;
            if ((flags << 30) >= 0) {
                goto zeroA;
            }
            c = rec[1636 + i];
            if (c == 0) {
zeroA:
                q[8707] |= 0x80;
                q[8705] = 4;
                q[9591] &= 0x7F;
            } else {
                q[8707] &= 0x7F;
                q[9591] |= 0x80;
                if (c != 0) {
                    *(s16 *)(q + 9612) = (c & 7) + 88;
                }
            }
            q += 68;
        }

        ent = rec + 448;
        q = D_801273D0_b;
        for (i = 8; i != 11; i++) {
            flags = *(s32 *)(ent + 788);
            ent += 56;
            if ((flags << 30) >= 0) {
                goto zeroB;
            }
            c = rec[1636 + i];
            if (c == 0) {
zeroB:
                q[8775] |= 0x80;
                q[8773] = 4;
                q[9591] &= 0x7F;
            } else {
                q[8775] &= 0x7F;
                q[9591] |= 0x80;
                if (c != 0) {
                    *(s16 *)(q + 9612) = (c & 7) + 88;
                }
            }
            q += 68;
        }
    }

    cw140_disp |= 0x80;
    cw140_prio = 4;
    cw136_disp |= 0x80;
    cw136_prio = 4;
}
