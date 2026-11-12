/* Function: date_effect_fall */
/* Overlay: DATE */
/* Address: 0x801560C8 */
/* Size: 628 bytes (157 insns) */
/* so `@CompileFlags: -Wc,-pic1` is stamped explicitly (a patch_stack */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 136 --new 144 */
/* @PostProc: patch_insn_reg_field 0x124:rt:t1:t3 0x138:rt:t2:t1 0x13c:rt:t1:t3 0x140:rs:t2:t1 0x140:rt:t3:t2 0x144:rt:t3:t2 0x158:rs:v0:s5 0x158:rt:s5:v0 0x190:rs:v0:s5 0x190:rt:s5:v0 0x1c4:rs:v0:s6 0x1c4:rt:s6:v0 0x1d0:rt:t1:t0 0x1d8:rs:t8:t0 0x1d8:rt:t1:t8 */
#include "types.h"

typedef struct {
      u8  unk00;
      u8  unk01;
      u8  unk02;
      u8  unk03;
      u8  unk04;
      u8  unk05;
      u8  unk06;
      u8  unk07;
      s16 unk08;
      s16 unk0A;
      s32 unk0C;
      s32 unk10;
      s16 unk14;
      s16 unk16;
      s16 unk18;
      s16 unk1A;
      s32 unk1C;
      s32 unk20;
      s16 unk24;
      s16 unk26;
      s16 unk28;
      s16 unk2A;
      s32 unk2C;
      s32 unk30;
      s32 unk34;
      s32 unk38;
      s32 unk3C;
      u8  unk40;
      u8  unk41;
      u8  unk42;
      u8  unk43;
} DateEnt;

typedef struct {
    s16 unk00;
    s16 unk02;
} Pair;

typedef struct {
    Pair p[8];
} PairTbl;

extern DateEnt class_w[];
extern PairTbl date_effect_fall_tbl;
extern u8  date_fall_st[];
extern s16 date_fall_y_tbl[];
extern u8  date_fall_wait_cnt[];

extern s32 rand(void);

void date_effect_fall(void)
{
    PairTbl local;
    s16 i;
    u32 v;

    local = date_effect_fall_tbl;

    for (i = 0; i < 8; i++) {
        class_w[i + 98].unk07 = class_w[96].unk07;
        v = date_fall_st[i];

        if (v == 0U) {
            date_fall_wait_cnt[i]++;
        }
        if ((v == 0U) && (date_fall_wait_cnt[i] >= 121U) &&
            ((u32)(180 - date_fall_wait_cnt[i]) < (u32)(rand() % 60))) {
            class_w[i + 98].unk02 = 5;
            class_w[i + 98].unk03 |= 0x80;
            date_fall_st[i] = 1;
        }

        v = date_fall_st[i];
        if (i == 0) {
            if ((v == 1U) && (class_w[i + 98].unk18 == 7) &&
                (class_w[i + 98].unk08 == 0)) {
                class_w[i + 98].unk30 = 0;
                date_fall_st[i] = 2;
                v = date_fall_st[i];
            }
        } else {
            if ((v == 1U) && (class_w[i + 98].unk18 == 3) &&
                (class_w[i + 98].unk08 == 7)) {
                class_w[i + 98].unk30 = 0;
                date_fall_st[i] = 2;
                v = date_fall_st[i];
            }
        }

        if (v == 2U) {
            class_w[i + 98].unk30 = class_w[i + 98].unk30 + 16384;
            *(s32 *)&class_w[i + 98].unk28 =
                *(s32 *)&class_w[i + 98].unk28 +
                class_w[i + 98].unk30;

            if ((local.p[i].unk02 - 120) < class_w[i + 98].unk2A) {
                class_w[i + 98].unk03 &= 0x7F;
                class_w[i + 98].unk02 = 0;
                class_w[i + 98].unk2A = date_fall_y_tbl[i] - 120;
                class_w[i + 98].unk28 = 0;
                class_w[i + 98].unk18 = 0;
                class_w[i + 98].unk08 = 0;
                date_fall_st[i] = 0;
                date_fall_wait_cnt[i] = 0;
            }
        }
    }
}
