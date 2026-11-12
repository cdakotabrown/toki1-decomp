/* Address: 0x8013A55C */
/* Size: 808 bytes (202 insns) */
/* @PostProc: patch_stack_complete --old 136 --new 152 */
/* @PostProc: remap_stack_slots --map '104:116,72:84,56:68' */
/* @PostProc: patch_insn_reg_field 0xe4:rt:t3:t2 0xe8:rt:t2:t1 0xec:rt:t1:t0 0xf4:rt:t0:t4 0xf8:rs:t1:t0 0xf8:rt:t1:t0 0xfc:rs:t2:t1 0xfc:rt:t2:t1 0x100:rs:t3:t2 0x100:rt:t3:t2 0x104:rs:t0:t4 0x104:rt:t0:t4 0x124:rt:t4:t3 0x154:rt:t4:t3 0x160:rt:t1:t0 0x164:rt:t2:t1 0x168:rt:t0:t4 0x16c:rt:t3:t2 */
#include "types.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  f02;
    u8  f03;
    u8  pad04;
    u8  f05;
    u8  pad06;
    u8  f07;
    u8  pad08[4];
    s32 f12;
    s32 f16;
    s16 f20;
    u8  pad22[2];
    s16 f24;
    s16 f26;
    s16 f28;
    u8  pad30[8];
    s16 f38;
    u8  pad40[2];
    s16 f42;
    u8  pad44[8];
    s32 f52;
    s32 f56;
    u8  pad60[7];
    u8  f67;
} Rec68;

typedef struct {
    s16 v[16];
} Buf32;

typedef struct {
    u8 v[16];
} Buf16;

extern Rec68 class_w[];
extern u8  taiiku_tama_field_tpg[];
extern s32 taiiku_tama_girl;
extern Buf32 taiiku_field_init_tbl0;
extern Buf32 taiiku_field_init_tbl1;
extern Buf16 taiiku_field_init_tbl2;
extern s32 taiiku_spr_img_11;
extern s32 taiiku_spr_anim_11;
extern s32 taiiku_spr_aux[];

extern void class_clear_f(void);

void taiiku_field_init(void)
{
    Buf32 posY;
    Buf32 posX;
    Buf16 kind;
    s32 i;

    posY = taiiku_field_init_tbl0;
    posX = taiiku_field_init_tbl1;
    kind = taiiku_field_init_tbl2;
    class_clear_f();
    for (i = 0; i < 16; i++) {
        class_w[96 + i].f56 = 0x01000000;
        class_w[96 + i].f07 = 128;
        class_w[96 + i].f05 = 128;
        class_w[96 + i].f12 = taiiku_spr_img_11;
        class_w[96 + i].f16 = taiiku_spr_anim_11;
        class_w[96 + i].f52 = (s32)taiiku_tama_field_tpg;
        class_w[96 + i].f20 = (*(s16 *)&taiiku_spr_aux[11]);
        class_w[96 + i].f26 = 4096;
        class_w[96 + i].f28 = 4096;
        class_w[96 + i].f02 = 64;
        class_w[96 + i].f03 = 132;
        class_w[96 + i].f01 = (u8)(kind.v[i] + 240);
        class_w[96 + i].f38 = posY.v[i];
        class_w[96 + i].f42 = posX.v[i];
    }
    class_w[96].f24 = 0;
    class_w[96].f01 = 3;
    class_w[97].f24 = (s16)(taiiku_tama_girl * 6 + 6);
    class_w[97].f01 = 3;
    class_w[98].f24 = 130;
    class_w[98].f07 = 104;
    class_w[98].f56 = class_w[98].f56 | 0x40000000;
    class_w[99].f24 = 131;
    class_w[99].f07 = 104;
    class_w[99].f56 = class_w[99].f56 | 0x40000000;
    class_w[100].f24 = 148;
    class_w[101].f01 = 1;
    class_w[101].f03 = 4;
    class_w[101].f24 = 132;
    class_w[102].f24 = 133;
    class_w[102].f03 = 4;
    class_w[103].f24 = 129;
    class_w[103].f07 = 104;
    class_w[103].f56 = class_w[103].f56 | 0x40000000;
    class_w[104].f24 = 140;
    class_w[104].f03 = (u8)(class_w[104].f03 | 1);
    for (i = 0; i < 3; i++) {
        class_w[105 + i].f24 = 134;
        class_w[105 + i].f03 = (u8)(class_w[105 + i].f03 | 1);
        class_w[105 + i].f67 = (u8)(i + 28);
    }
    class_w[108].f24 = 146;
    for (i = 0; i < 3; i++) {
        class_w[109 + i].f24 = 147;
        class_w[109 + i].f67 = (u8)(i + 28);
    }
}
