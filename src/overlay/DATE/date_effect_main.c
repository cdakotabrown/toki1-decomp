/* Function: date_effect_main */
/* Overlay: DATE */
/* Address: 0x80155E28 */
/* Size: 484 bytes (121 insns) */
/* @SymbolSize: 484 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=3 IDO52_AS1_NO_FALLTHRU_FILL=4 IDO52_EXCLUDE_REGS_WEB=8:v1 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x14c */
/* @PostProc: patch_insn_reg_field 0x188:rs:t5:t3 0xd4:rt:t3:a2 0xe0:rs:t3:a2 0xe0:rt:t3:a2 0xe8:rs:t3:a2 0xfc:rt:t4:a2 0x100:rs:t4:a2 0x100:rt:t4:a2 0x108:rd:t5:t3 0x108:rt:t4:a2 0x10c:rt:t5:t3 0x128:rt:t8:t4 0x12c:rd:t0:t5 0x130:rd:t7:t8 0x130:rt:t0:t5 0x134:rs:t7:t8 0x134:rt:t7:t8 0x138:rs:t8:t4 0x138:rt:t8:t4 0x13c:rs:t8:t4 0x13c:rt:t7:t8 0x158:rd:t3:t1 0x15c:rt:t6:t0 0x160:rs:t6:t0 0x160:rt:t6:t0 0x164:rt:t3:t1 0x16c:rd:t1:t7 0x170:rd:t2:t6 0x170:rt:t1:t7 0x178:rs:t2:t6 0x178:rt:t6:t0 0x17c:rt:t5:t3 0x180:rs:t5:t3 0x180:rt:t5:t3 0x1a0:rd:t7:t8 0x1a4:rt:t7:t8 0x1b4:rt:t0:t5 0x1bc:rs:t0:t5 0x1bc:rt:t8:t4 0x1c4:rt:t8:t4 */
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
      s32 unk08;
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
    void (*f[45])(s32);
} FnTable;

extern FnTable date_effect_main_tbl;
extern DateEnt class_w[];

extern u8  now_year;
extern u8  now_month;
extern u8  now_day;
extern u8  now_sub_sub_step;
extern u8  fade_bri;
extern s32 date_sprite_on;

extern void date_light_effect(void);
extern void date_fade_out(s32);
extern void date_effect_fall(void);

void date_effect_main(void)
{
    s16 i;
    u32 scene;
    FnTable local;

    scene = now_year;
    local = date_effect_main_tbl;

    if (((scene == 95) && (now_month == 7) && (now_day == 23)) ||
        ((scene == 96) && (now_month == 7) && (now_day == 28)) ||
        ((scene == 97) && (now_month == 7) && (now_day == 27))) {
        if ((fade_bri == 128) && (date_sprite_on == 0) && (now_sub_sub_step >= 33U)) {
            date_light_effect();
        }
    }

    local.f[now_sub_sub_step](128);

    if (local.f[now_sub_sub_step] == date_fade_out) {
        for (i = 0; i < 8; i++) {
            class_w[i + 98].unk07 = class_w[96].unk07;
        }
        if (class_w[96].unk07 < 9U) {
            for (i = 0; i < 8; i++) {
                class_w[i + 98].unk03 &= 0x7F;
            }
        }
    }

    if ((now_sub_sub_step >= 17U) && (now_sub_sub_step < 20U)) {
        date_effect_fall();
    }
}
