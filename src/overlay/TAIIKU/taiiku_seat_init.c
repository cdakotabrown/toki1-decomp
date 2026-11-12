/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x80132BD0 */
/* Size: 592 bytes (148 insns) */
#include "types.h"

typedef struct { u8 b[23]; } Rec23;
typedef struct { s16 h[23]; } Rec46;

extern void class_clear_n(s32);

extern Rec23 taiiku_seat_init_tbl0;
extern Rec46 taiiku_seat_init_tbl1;
extern Rec46 taiiku_seat_init_tbl2;
extern Rec46 taiiku_seat_init_tbl3;
extern s32 taiiku_spr_img_7;
extern s32 taiiku_spr_anim_7;
extern u8 taiiku_com_tpg[];
extern u8 class_w[][68];
extern s32 taiiku_spr_aux[];

void taiiku_seat_init(void) {
    u8 pad24[24];
    Rec23 buf248;
    Rec46 buf200;
    Rec46 buf152;
    Rec46 buf104;
    u8 pad8[8];
    s32 i;

    buf248 = taiiku_seat_init_tbl0;
    buf200 = taiiku_seat_init_tbl1;
    buf152 = taiiku_seat_init_tbl2;
    buf104 = taiiku_seat_init_tbl3;

    for (i = 0; i != 23; i++) {
        class_clear_n(i + 1);
        *(u8 *)(class_w[i + 1] + 1) = buf248.b[i];
        *(u32 *)(class_w[i] + 124) = 0x01000000;
        *(u8 *)(class_w[i + 1] + 4) = 0;
        *(u8 *)(class_w[i + 1] + 5) = 128;
        *(u8 *)(class_w[i + 1] + 6) = 2;
        *(s16 *)(class_w[i + 1] + 8) = 0;
        *(s32 *)(class_w[i + 1] + 12) = taiiku_spr_img_7;
        *(s32 *)(class_w[i + 1] + 16) = taiiku_spr_anim_7;
        *(u8 **)(class_w[i + 1] + 52) = taiiku_com_tpg;
        *(s16 *)(class_w[i + 1] + 20) = (*(s16 *)&taiiku_spr_aux[7]);
        *(s16 *)(class_w[i + 1] + 26) = 4096;
        *(s16 *)(class_w[i + 1] + 28) = 4096;
        *(u8 *)(class_w[i + 1] + 7) = 128;
        *(s16 *)(class_w[i + 1] + 38) = buf200.h[i] - 160;
        *(s16 *)(class_w[i + 1] + 42) = buf152.h[i] - 120;
        *(s16 *)(class_w[i + 1] + 24) = 0;
        *(s16 *)(class_w[i + 1] + 22) = buf104.h[i];
    }
}
