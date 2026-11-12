/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:5 */
/* Address: 0x8014850C */
/* Size: 596 bytes (149 insns) */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x34 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0xBC --offset2 0xC0 */
/* @PostProc: swap_instructions --offset1 0xC0 --offset2 0xC4 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t4:t5 0x34:rt:t5:t3 0x44:rs:t5:t3 0x44:rt:t5:t3 0x48:rs:t4:t5 0x48:rt:t4:t5 0x50:rt:t3:t4 0x6c:rt:t4:t5 0x88:rt:t5:t3 0xa4:rt:t3:t4 0xac:rt:a0:a1 0xb0:rs:a0:a1 0xb0:rt:a0:a1 0xb8:rt:v1:a0 0xbc:rt:a2:a3 0xc0:rt:a1:a2 0xc4:rt:a3:v1 0xc8:rs:a0:a1 0xcc:rt:v1:a0 0xd0:rt:a3:v1 0xd4:rs:v1:a0 0xd4:rt:v1:a0 0xd8:rs:v1:a0 0xe0:rs:a0:a1 0xe0:rt:a0:a1 0xe4:rt:a1:a2 0xe8:rt:a2:a3 0x108:rt:a3:v1 0x13c:rt:a3:v1 0x184:rt:a3:v1 0x1cc:rt:a3:v1 */
#include "types.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  f02;
    u8  f03;
    u8  f04;
    u8  f05;
    u8  f06;
    u8  f07;
    u8  pad08[4];
    s32 f12;
    s32 f16;
    s16 f20;
    s16 f22;
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
extern s32 taiiku_spr_img_10;
extern s32 taiiku_spr_anim_10;
extern u8  taiiku_spoon_rank[];
extern u8  taiiku_spoon_scene_tpg[];
extern s32 taiiku_spr_aux[];

extern void class_clear_f(void);

void taiiku_scenery_init(void)
{
    s32 i;
    s32 x;

    class_clear_f();
    for (i = 0; i < 9; i++) {
        class_w[96 + i].f56 = 0x01000000;
        class_w[96 + i].f07 = 128;
        class_w[96 + i].f02 = 64;
        class_w[96 + i].f03 = 164;
        class_w[96 + i].f12 = taiiku_spr_img_10;
        class_w[96 + i].f16 = taiiku_spr_anim_10;
        class_w[96 + i].f52 = (s32)taiiku_spoon_scene_tpg;
        class_w[96 + i].f20 = (*(s16 *)&taiiku_spr_aux[10]);
        class_w[96 + i].f04 = 0;
        class_w[96 + i].f05 = 128;
        class_w[96 + i].f26 = 4096;
        class_w[96 + i].f28 = 4096;
        class_w[96 + i].f06 = 2;
    }
    i = 0;
    for (x = -112; x < 80; x += 48) {
        class_w[96 + i].f38 = (s16)x;
        i++;
        class_w[95 + i].f01 = 3;
        class_w[95 + i].f42 = -8;
        class_w[95 + i].f24 = (s16)(taiiku_spoon_rank[i - 1] * 5 + 29);
    }
    class_w[100].f01 = 3;
    class_w[100].f24 = (s16)(taiiku_spoon_rank[0] * 5 + 13);
    class_w[100].f38 = class_w[96].f38;
    class_w[100].f42 = class_w[96].f42;
    class_w[101].f01 = 3;
    class_w[101].f24 = (s16)(taiiku_spoon_rank[1] * 5 + 13);
    class_w[101].f38 = class_w[97].f38;
    class_w[101].f42 = class_w[97].f42;
    class_w[102].f01 = 3;
    class_w[102].f24 = (s16)(taiiku_spoon_rank[2] * 5 + 13);
    class_w[102].f38 = class_w[98].f38;
    class_w[102].f42 = class_w[98].f42;
    class_w[103].f01 = 3;
    class_w[103].f24 = (s16)(taiiku_spoon_rank[3] * 5 + 13);
    class_w[103].f38 = class_w[99].f38;
    class_w[103].f42 = class_w[99].f42;
    class_w[104].f01 = 3;
    class_w[104].f22 = 9;
    class_w[104].f38 = 192;
    class_w[104].f42 = -32;
    class_w[104].f02 = 1;
}
