/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:8 */
/* Address: 0x80145000 */
/* Size: 496 bytes (124 insns) */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0xC0 --offset2 0xC4 */
/* @PostProc: swap_instructions --offset1 0xC4 --offset2 0xC8 */
/* @PostProc: swap_instructions --offset1 0xC8 --offset2 0xCC */
/* @PostProc: swap_branch_operands --offsets 0x104 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t5:ra 0x34:rt:ra:t4 0x44:rs:ra:t4 0x44:rt:ra:t4 0x48:rs:t5:ra 0x48:rt:t5:ra 0x50:rt:t4:t5 0x70:rt:t5:ra 0x94:rt:ra:t4 0xa8:rt:t4:t5 */
#include "types.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  f02;
    u8  f03;
    u8  pad04;
    u8  f05;
    u8  f06;
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
    u8  pad60[8];
} Rec68;

extern Rec68 class_w[];
extern s32 taiiku_spr_img_6;
extern s32 taiiku_spr_anim_6;
extern u8  taiiku_ninin_girl1;
extern u8  taiiku_ninin_rank[];
extern u8  taiiku_ninin_rank_tpg[];
extern s32 taiiku_spr_aux[];

extern void class_clear_f(void);

void taiiku_rank_sprite_init(void)
{
    s32 i;
    s32 x;

    class_clear_f();
    for (i = 0; i < 8; i++) {
        class_w[96 + i].f03 = 164;
        class_w[96 + i].f02 = 64;
        class_w[96 + i].f05 = 128;
        class_w[96 + i].f01 = 3;
        class_w[96 + i].f56 = 0x01000000;
        class_w[96 + i].f07 = 128;
        class_w[96 + i].f12 = taiiku_spr_img_6;
        class_w[96 + i].f16 = taiiku_spr_anim_6;
        class_w[96 + i].f52 = (s32)taiiku_ninin_rank_tpg;
        class_w[96 + i].f20 = (*(s16 *)&taiiku_spr_aux[6]);
        class_w[96 + i].f26 = 4096;
        class_w[96 + i].f28 = 4096;
        class_w[96 + i].f06 = 2;
    }
    x = -120;
    for (i = 0; i < 4; i++) {
        class_w[96 + i].f38 = (s16)x;
        class_w[96 + i].f24 = (s16)(taiiku_ninin_rank[i] * 8 + 26);
        x += 64;
        class_w[96 + i].f42 = -8;
        if (class_w[96 + i].f24 == 34 && taiiku_ninin_girl1 == 1) {
            class_w[96 + i].f24 = 66;
        }
    }
    class_w[100].f38 = -120;
    class_w[100].f42 = -8;
    class_w[100].f24 = (s16)(taiiku_ninin_rank[0] * 5 + 13);
    class_w[101].f38 = -56;
    class_w[101].f42 = -8;
    class_w[101].f24 = (s16)(taiiku_ninin_rank[1] * 5 + 13);
    class_w[102].f38 = 8;
    class_w[102].f42 = -8;
    class_w[102].f24 = (s16)(taiiku_ninin_rank[2] * 5 + 13);
    class_w[103].f38 = 72;
    class_w[103].f42 = -8;
    class_w[103].f24 = (s16)(taiiku_ninin_rank[3] * 5 + 13);
}
