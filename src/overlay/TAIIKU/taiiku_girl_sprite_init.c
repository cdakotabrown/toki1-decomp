/* Address: 0x8013A07C */
/* Size: 432 bytes (108 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

typedef struct { s16 h[11]; } Rec2;
typedef struct { s32 f0; u8 pad[52]; } Ent56;

extern Rec2 taiiku_girl_sprite_init_tbl;
extern u8 class_w[];
extern s32 taiiku_spr_img_12;
extern s32 taiiku_spr_anim_12;
extern u8 taiiku_tama_girl_tpg[];
extern Ent56 girl0_flag[];
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 taiiku_spr_aux[];

extern void class_clear_n();

void taiiku_girl_sprite_init(void) {
    u8 *p;
    s32 i;
    s32 j;
    Rec2 buf;
    s32 bp;

    bp = (s32)&buf;
    buf = taiiku_girl_sprite_init_tbl;
    j = 0;
    p = class_w;
    for (i = 0; i != 12; i++) {
        class_clear_n(i + 96);
        *(s32 *)(p + 6584) = 0x01000000;
        *(u8 *)(p + 6535) = 128;
        *(s32 *)(p + 6540) = taiiku_spr_img_12;
        *(s32 *)(p + 6544) = taiiku_spr_anim_12;
        *(u8 **)(p + 6580) = taiiku_tama_girl_tpg;
        *(s16 *)(p + 6548) = (*(s16 *)&taiiku_spr_aux[12]);
        *(u8 *)(p + 6533) = 128;
        *(s16 *)(p + 6554) = 4096;
        *(s16 *)(p + 6556) = 4096;
        *(u8 *)(p + 6534) = 2;
        *(u8 *)(p + 6530) = 1;
        if (i == 11) {
            *(s16 *)(p + 6566) = 0;
            *(s16 *)(p + 6570) = 0;
            *(u8 *)(p + 6529) = 2;
            *(u8 *)(p + 6531) = 164;
        } else {
            if (((u32)(girl0_flag[i].f0 << 30) >> 31) == 1) {
                *(s16 *)(p + 6566) = ((s16 *)bp)[j];
                *(u8 *)(p + 6531) = 164;
                j++;
            } else {
                *(s16 *)(p + 6566) = 200;
                *(u8 *)(p + 6531) = 0;
            }
            *(s16 *)(p + 6570) = 40;
            *(u8 *)(p + 6529) = 3;
        }
        *(s16 *)(p + 6550) = (s16)i;
        p += 68;
    }
    cursor_cw_x = 0;
    cursor_cw_y = 0;
}
