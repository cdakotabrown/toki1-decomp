/* @CompileEnv: IDO52_AS1_NO_FALLTHRU_FILL=occ:1 */
/* Address: 0x8013625C */
/* Size: 808 bytes (202 instructions) */
/* NOTE for audit_raw_count.py: it scrapes EVERY "@CompileEnv:" occurrence in */
/* @PostProc: remap_stack_slots --map '72:76' */
#include "types.h"

typedef struct {
    s32 flags;
    s16 x;
    s16 y;
    u16 unk08;
    u16 unk0A;
    s16 unk0C;
    u8  unk0E;
    u8  unk0F;
    s16 unk10;
    s16 unk12;
    u8  r;
    u8  g;
    u8  b;
    u8  pad17;
    s16 unk18;
    s16 unk1A;
    s16 sx;
    s16 sy;
    s32 unk20;
} EventObj;

extern s32 func_800789A0(void);

extern EventObj D_800EC190[];
extern s16 D_80120CF0[];
extern s16 D_80120D10[];

void func_8013625C(void)
{
    s16 i;
    s16 av;
    s16 bv;

    for (i = 4; i < 19; i++) {
        D_800EC190[i].y += 10;

        if (((func_800789A0() & 3) == 0 && D_800EC190[i].y >= 17 &&
             (av = D_80120CF0[i - 4]) == 0 &&
             D_80120D10[i - 4] == 0) ||
            (av = D_80120CF0[i - 4]) != 0 || D_80120D10[i - 4] != 0) {
            D_800EC190[i].y -= 10;
            if (av == 0) {
                if (D_80120D10[i - 4] == 0) {
                    if (D_800EC190[i].x < -87) {
                        D_800EC190[i].y = func_800789A0() % 4 + 28;
                    } else {
                        D_800EC190[i].y = func_800789A0() % 15 + 17;
                    }
                    D_800EC190[i].x -= 12;
                    D_800EC190[i].unk0E = 8;
                    D_800EC190[i].unk08 = 24;
                    D_800EC190[i].unk0A = 8;
                    D_80120D10[i - 4] = D_80120D10[i - 4] + 1;
                } else {
                    D_80120D10[i - 4] = 0;
                    D_80120CF0[i - 4] = av + 1;
                }
            } else if (av == 5) {
                D_800EC190[i].unk0E = 0;
                D_800EC190[i].unk08 = 8;
                D_800EC190[i].unk0A = 24;
                D_800EC190[i].x = func_800789A0() % 167 - 159;
                if (D_800EC190[i].x >= -87) {
                    D_800EC190[i].x += 153;
                }
                D_800EC190[i].y = -95;
                D_80120CF0[i - 4] = 0;
                D_80120D10[i - 4] = 0;
            } else {
                bv = D_80120D10[i - 4];
                if (av == 4) {
                    if (bv == 0) {
                        D_800EC190[i].unk0E -= 24;
                        D_80120D10[i - 4] = bv + 1;
                    } else {
                        D_80120D10[i - 4] = 0;
                        D_80120CF0[i - 4] = av + 1;
                    }
                } else {
                    if (bv == 0) {
                        D_800EC190[i].unk0E += 24;
                        D_80120D10[i - 4] = bv + 1;
                    } else {
                        D_80120D10[i - 4] = 0;
                        D_80120CF0[i - 4] = av + 1;
                    }
                }
            }
        } else if (D_800EC190[i].y >= 17 && av == 0 && D_80120D10[i - 4] == 0) {
            D_800EC190[i].x = func_800789A0() % 167 - 159;
            if (D_800EC190[i].x >= -87) {
                D_800EC190[i].x += 153;
            }
            D_800EC190[i].y = -95;
        }
    }
}
