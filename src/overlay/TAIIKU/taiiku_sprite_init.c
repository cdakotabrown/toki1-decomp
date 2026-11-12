/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_UGEN_TEMP_REG=33:t8,43:t9 IDO52_AS1_TIE_REJECT=1572 */
/* @SymbolSize: 1116 */
/* taiiku_sprite_init -- TAIIKU overlay, Address: 0x80138310, Size: 1116 bytes */
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
    s16 f08;
    s16 pad10;
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
    s32 *f52;
    s32 f56;
    u8  pad60[7];
    u8  f67;
} Rec68;

typedef struct { u16 h[5]; } Blk10;
typedef struct { s32 w[5]; } Blk20;

extern Rec68 class_w[];
extern Blk10 taiiku_sprite_init_tbl0;
extern Blk20 taiiku_sprite_init_tbl1;
extern Blk20 taiiku_sprite_init_tbl2;
extern Blk20 taiiku_sprite_init_tbl3;
extern s32 taiiku_spr_img_2;
extern s32 taiiku_spr_img_3;
extern s32 taiiku_spr_anim_2;
extern s32 taiiku_spr_anim_3;
extern s32 taiiku_kari_runner_tpg[];
extern s32 taiiku_kari_sprite_tpg[];
extern s32 taiiku_spr_aux[];

extern get_taiiku_rank_frame();
extern get_taiiku_rank_x();
extern class_clear_n();

taiiku_sprite_init()
{
    s32 i;
    s32 k;
    Blk10 b1;
    Blk20 b2;
    Blk20 b3;
    Blk20 b4;
    u8  *p1;
    s32 y;

    b1 = taiiku_sprite_init_tbl0;
    b2 = taiiku_sprite_init_tbl1;
    b3 = taiiku_sprite_init_tbl2;
    b4 = taiiku_sprite_init_tbl3;

    for (i = 0, y = 40, k = 2; i < 4; y -= 24, i++, k += 2) {
        class_w[96 + i].f56 = 0x01000000;
        class_w[96 + i].f01 = (u8)k;
        class_w[96 + i].f04 = 0;
        class_w[96 + i].f05 = 128;
        class_w[96 + i].f02 = 64;
        class_w[96 + i].f03 = 164;
        class_w[96 + i].f06 = 2;
        class_w[96 + i].f08 = 0;
        class_w[96 + i].f12 = taiiku_spr_img_3;
        class_w[96 + i].f16 = taiiku_spr_anim_3;
        class_w[96 + i].f52 = taiiku_kari_sprite_tpg;
        class_w[96 + i].f20 = (*(s16 *)&taiiku_spr_aux[3]);
        class_w[96 + i].f22 = 0;
        class_w[96 + i].f24 = (s16)get_taiiku_rank_frame(i, 1);
        class_w[96 + i].f26 = 4096;
        class_w[96 + i].f28 = 4096;
        class_w[96 + i].f07 = 128;
        class_w[96 + i].f38 = (s16)get_taiiku_rank_x(i);
        class_w[96 + i].f42 = (s16)y;
    }

    for (i = 0, y = 40, k = 3; i < 4; y -= 24, i++, k += 2) {
        class_w[107 + i].f56 = 0x01000000;
        class_w[107 + i].f01 = (u8)k;
        class_w[107 + i].f04 = 0;
        class_w[107 + i].f05 = 128;
        class_w[107 + i].f02 = 64;
        class_w[107 + i].f03 = 164;
        class_w[107 + i].f06 = 2;
        class_w[107 + i].f08 = 0;
        class_w[107 + i].f12 = taiiku_spr_img_3;
        class_w[107 + i].f16 = taiiku_spr_anim_3;
        class_w[107 + i].f52 = taiiku_kari_sprite_tpg;
        class_w[107 + i].f20 = (*(s16 *)&taiiku_spr_aux[3]);
        class_w[107 + i].f22 = 0;
        class_w[107 + i].f24 = (s16)get_taiiku_rank_frame(i, 0);
        class_w[107 + i].f26 = 4096;
        class_w[107 + i].f28 = 4096;
        class_w[107 + i].f07 = 128;
        class_w[107 + i].f38 = (s16)get_taiiku_rank_x(i);
        class_w[107 + i].f42 = (s16)y;
    }

    for (i = 0, p1 = (u8 *)&b1; i != 10; p1++, i++) {
        class_clear_n(i + 111);
        class_w[111 + i].f01 = *p1;
        class_w[111 + i].f56 = 0x01000000;
        class_w[111 + i].f04 = 0;
        class_w[111 + i].f05 = 128;
        class_w[111 + i].f02 = 1;
        class_w[111 + i].f03 = 164;
        class_w[111 + i].f06 = 2;
        class_w[111 + i].f08 = 0;
        class_w[111 + i].f12 = taiiku_spr_img_3;
        class_w[111 + i].f16 = taiiku_spr_anim_3;
        class_w[111 + i].f52 = taiiku_kari_sprite_tpg;
        class_w[111 + i].f20 = (*(s16 *)&taiiku_spr_aux[3]);
        class_w[111 + i].f22 = ((s16 *)&b2)[i];
        class_w[111 + i].f38 = ((s16 *)&b3)[i];
        class_w[111 + i].f42 = ((s16 *)&b4)[i];
        class_w[111 + i].f24 = 0;
        class_w[111 + i].f26 = 4096;
        class_w[111 + i].f28 = 4096;
        class_w[111 + i].f67 = 0;
        class_w[111 + i].f07 = 128;
    }

    class_clear_n(112);
    class_w[112].f56 = 0x01000000;
    class_w[112].f01 = 1;
    class_w[112].f04 = 0;
    class_w[112].f05 = 128;
    class_w[112].f02 = 64;
    class_w[112].f03 = 164;
    class_w[112].f06 = 2;
    class_w[112].f08 = 0;
    class_w[112].f12 = taiiku_spr_img_2;
    class_w[112].f16 = taiiku_spr_anim_2;
    class_w[112].f52 = taiiku_kari_runner_tpg;
    class_w[112].f20 = (*(s16 *)&taiiku_spr_aux[2]);
    class_w[112].f22 = 0;
    class_w[112].f24 = 160;
    class_w[112].f26 = 8192;
    class_w[112].f28 = 8192;
    class_w[112].f67 = 0;
    class_w[112].f07 = 128;
    class_w[112].f38 = -128;
    class_w[112].f42 = -248;
}
