/* @PostProc: remap_stack_slots --from 80 --to 84 */
/* Address: 0x80141DF8 */
/* Size: 856 bytes (214 instructions) */
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

void func_80141DF8(void)
{
    s16 i;
    s16 av;
    s16 bv;

    for (i = 4; i < 14; i++) {
        D_800EC190[i].y += 10;

        if (((func_800789A0() & 3) == 0 &&
             ((D_800EC190[i].y >= -82 && D_800EC190[i].y < -57 && D_800EC190[i].x < -87) ||
              (D_800EC190[i].y >= -57 && D_800EC190[i].y < -26 && D_800EC190[i].x < -119) ||
              (D_800EC190[i].y >= -11 && D_800EC190[i].y < 3 && D_800EC190[i].x >= -141 &&
               D_800EC190[i].x < -111) ||
              (D_800EC190[i].y >= 5 && D_800EC190[i].x >= -127 && D_800EC190[i].x < -92)) &&
             (av = D_80120CF0[i - 4]) == 0 &&
             D_80120D10[i - 4] == 0) ||
            (av = D_80120CF0[i - 4]) != 0 || D_80120D10[i - 4] != 0) {
            D_800EC190[i].y -= 10;
            if (av == 0) {
                bv = D_80120D10[i - 4];
                if (bv == 0) {
                    if (D_800EC190[i].y < -68) {
                        D_800EC190[i].unk0F += 8;
                    }
                    D_800EC190[i].x -= 12;
                    D_800EC190[i].y += 16;
                    D_800EC190[i].unk0E = 8;
                    D_800EC190[i].unk08 = 24;
                    D_800EC190[i].unk0A = 8;
                    D_80120D10[i - 4] = bv + 1;
                } else {
                    D_80120D10[i - 4] = 0;
                    D_80120CF0[i - 4] = av + 1;
                }
            } else if (av == 4) {
                D_800EC190[i].unk0E = 0;
                D_800EC190[i].unk0F = 0;
                D_800EC190[i].unk08 = 8;
                D_800EC190[i].unk0A = 24;
                D_800EC190[i].x = func_800789A0() % 84 - 159;
                D_800EC190[i].y = -120;
                D_80120D10[i - 4] = 0;
                D_80120CF0[i - 4] = 0;
            } else {
                bv = D_80120D10[i - 4];
                if (av == 5) {
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
        } else if (D_800EC190[i].y >= 25 && av == 0 && D_80120D10[i - 4] == 0) {
            D_800EC190[i].x = func_800789A0() % 84 - 159;
            D_800EC190[i].y = -120;
        }
    }
}
