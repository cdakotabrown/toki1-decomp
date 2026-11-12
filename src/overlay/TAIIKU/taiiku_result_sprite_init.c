/* @CompileEnv: IDO52_UGEN_TEMP_REG=21:t6,22:t7,23:t9,24:t8,25:t6,26:t7,27:t9 */
/* Address: 0x80133258 */
/* Size: 480 bytes (120 insns) */
#include "types.h"

typedef struct { u8 b[10]; } Rec10;
typedef struct { s16 h[10]; } Rec20;

extern void class_clear_n(s32);

extern Rec10 taiiku_result_sprite_init_tbl0;
extern Rec20 taiiku_result_sprite_init_tbl1;
extern Rec20 taiiku_result_sprite_init_tbl2;
extern Rec20 taiiku_result_sprite_init_tbl3;
extern s32 taiiku_spr_img_7;
extern s32 taiiku_spr_anim_7;
extern u8 taiiku_com_tpg[];
extern u8 class_w[][68];
extern s32 taiiku_spr_aux[];

void taiiku_result_sprite_init(void) {
    s32 i;
    Rec10 buf136;
    Rec20 buf116;
    Rec20 buf96;
    Rec20 buf76;

    buf136 = taiiku_result_sprite_init_tbl0;
    buf116 = taiiku_result_sprite_init_tbl1;
    buf96 = taiiku_result_sprite_init_tbl2;
    buf76 = taiiku_result_sprite_init_tbl3;

    for (i = 0; i < 23; i++) {
        class_clear_n(i + 1);
    }

    for (i = 0; i < 10; i++) {
        *(u32 *)(class_w[i] + 124) = (u32)((i & 1) << 30) | 0x01000000;
        *(u8 *)(class_w[i + 1] + 5) = 128;
        *(u8 *)(class_w[i + 1] + 6) = 2;
        *(s32 *)(class_w[i + 1] + 12) = taiiku_spr_img_7;
        *(s32 *)(class_w[i + 1] + 16) = taiiku_spr_anim_7;
        *(u8 **)(class_w[i + 1] + 52) = taiiku_com_tpg;
        *(s16 *)(class_w[i + 1] + 20) = (*(s16 *)&taiiku_spr_aux[7]);
        *(s16 *)(class_w[i + 1] + 26) = 4096;
        *(s16 *)(class_w[i + 1] + 28) = 4096;
        *(u8 *)(class_w[i + 1] + 7) = 128;
        *(u8 *)(class_w[i + 1] + 2) = 1;
        *(u8 *)(class_w[i + 1] + 3) = 164;
        *(u8 *)(class_w[i + 1] + 1) = buf136.b[i];
        *(s16 *)(class_w[i + 1] + 38) = buf116.h[i];
        *(s16 *)(class_w[i + 1] + 42) = buf96.h[i];
        *(s16 *)(class_w[i + 1] + 22) = buf76.h[i];
    }
}
