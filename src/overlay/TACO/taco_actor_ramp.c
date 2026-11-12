/* @SymbolSize: 776 */
/* @PostProc: patch_insn_reg_field 0x30:rs:t6:s0 0x30:rt:s0:t6 0xa4:rs:t1:s0 0xa4:rt:s0:t1 0x114:rs:t4:s0 0x114:rt:s0:t4 0x144:rs:t9:s0 0x144:rt:s0:t9 0x180:rs:t3:s0 0x180:rt:s0:t3 0x200:rs:t4:s0 0x200:rt:s0:t4 0x248:rs:t1:s0 0x248:rt:s0:t1 0x264:rs:t4:s0 0x264:rt:s0:t4 0x2bc:rs:t3:s0 0x2bc:rt:s0:t3 0x2ec:rs:t9:s0 0x2ec:rt:s0:t9 */
/* Address: 0x8015A388 */
/* Size: 776 bytes (194 insns) */
#include "types.h"

typedef struct {
    u8 pad0[106];
    s16 f106;
    s16 f108;
    s16 f110;
    u8 pad1[4];
    s16 f116;
    s16 f118;
    s16 f120;
    s16 f122;
    s16 f124;
    u8 pad2[6];
    u8 f132;
    u8 pad3[3];
} Rec;

extern Rec *taco_actor_rec;

extern s32 rsin(s32);
extern s32 rcos(s32);
extern void taco_actor_exit(void);

void taco_actor_ramp(s32 idx)
{
    switch (taco_actor_rec[idx].f124) {
    case 0:
        taco_actor_rec[idx].f116 =
            (s16)((rsin(taco_actor_rec[idx].f106) * 1000) >> 12);
        taco_actor_rec[idx].f118 =
            (s16)(((rcos(taco_actor_rec[idx].f106) * 1000) >> 12) - 700);
        taco_actor_rec[idx].f120 = -20000;
        taco_actor_rec[idx].f110 = 1;
        taco_actor_rec[idx].f124 = taco_actor_rec[idx].f124 + 1;
        break;
    case 1:
        taco_actor_rec[idx].f110 = (s16)(taco_actor_rec[idx].f122 << 9);
        taco_actor_rec[idx].f132 = (u8)((taco_actor_rec[idx].f122 * 22) / 32);
        if (taco_actor_rec[idx].f110 >= taco_actor_rec[idx].f108) {
            taco_actor_rec[idx].f124 = taco_actor_rec[idx].f124 + 1;
            taco_actor_rec[idx].f122 = 0;
        }
        break;
    case 2:
        taco_actor_rec[idx].f116 =
            (s16)((rsin(taco_actor_rec[idx].f106) * 1000) >> 12);
        taco_actor_rec[idx].f118 =
            (s16)(((rcos(taco_actor_rec[idx].f106) * 1000) >> 12) - 700);
        taco_actor_rec[idx].f106 = taco_actor_rec[idx].f106 + 32;
        if (taco_actor_rec[idx].f122 >= 601) {
            taco_actor_rec[idx].f124 = taco_actor_rec[idx].f124 + 1;
            taco_actor_rec[idx].f122 = 0;
        }
        break;
    case 3:
        taco_actor_rec[idx].f110 = (s16)((16 - taco_actor_rec[idx].f122) << 9);
        taco_actor_rec[idx].f132 = (u8)(((16 - taco_actor_rec[idx].f122) * 32) / 32);
        if (taco_actor_rec[idx].f110 <= 0) {
            taco_actor_exit();
        }
        break;
    }
}
