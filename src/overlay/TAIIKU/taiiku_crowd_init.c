/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=bb:1 IDO52_CONST_NOINREG_VALUES=30 */
/* Address: 0x80135C14 */
/* Size: 596 bytes (149 insns) */
/* @PostProc: patch_stack_complete --old 160 --new 176 */
/* @PostProc: remap_stack_slots --map '116:120,160:164' */
/* @PostProc: patch_insn_reg_field 0x114:rt:fp:s7 0x118:rs:fp:s7 0x118:rt:fp:s7 0x124:rt:s7:fp 0x1b8:rt:fp:s7 0x1cc:rs:s7:fp */
#include "types.h"

typedef struct {
    u8  pad00;
    u8  f01;
    u8  pad02[2];
    u8  f04;
    u8  f05;
    u8  f06;
    u8  f07;
    s16 f08;
    u8  pad10[2];
    s32 f12;
    s32 f16;
    s16 f20;
    s16 f22;
    s16 f24;
    s16 f26;
    s16 f28;
    u8  pad30[12];
    s16 f42;
    u8  pad44[8];
    s32 f52;
    s32 f56;
    u8  pad60[8];
} Rec68;

typedef struct {
    u8  pad00[8];
    u32 b0 : 1;
    u32 b1 : 1;
    u32 b2 : 30;
    u8  pad12[44];
} Rec56;

typedef struct {
    s16 v[11];
} Tbl22;

extern Rec56 screen_w[];
extern Rec68 class_w[];
extern u8  taiiku_kari_crowd_tpg[];
extern s16 taiiku_kari_card;
extern Tbl22 taiiku_kari_card_map;
extern s32 taiiku_spr_img_1;
extern s32 taiiku_spr_anim_1;
extern s32 taiiku_spr_aux[];

extern s32  class_clear_n(s32 a0);
extern s32  rand(void);
extern s32  taiiku_get_crowd_x(void);

void taiiku_crowd_init(void)
{
    s32 arr[11];
    Tbl22 tbl;
    s32 i;
    s32 j;
    s32 cnt;
    s32 t;

    tbl = taiiku_kari_card_map;
    for (j = 0; j < 11; j++) {
        arr[j] = 128;
    }
    cnt = 0;
    for (i = 0; i < 11; i++) {
        if (screen_w[8 + i].b1 == 1) {
            arr[cnt] = i;
            cnt++;
        }
    }
    taiiku_kari_card = tbl.v[arr[rand() % cnt]];
    for (i = 0; i < 30; i++) {
        if (i < 10) {
            t = 0;
        } else if (i < 20) {
            t = 1;
        } else {
            t = 2;
        }
        class_clear_n(i + 46);
        class_w[46 + i].f56 = 0x01000000;
        class_w[46 + i].f01 = (u8)(t + 9);
        class_w[46 + i].f04 = 0;
        class_w[46 + i].f05 = 128;
        class_w[46 + i].f06 = 2;
        class_w[46 + i].f08 = 0;
        class_w[46 + i].f12 = taiiku_spr_img_1;
        class_w[46 + i].f16 = taiiku_spr_anim_1;
        class_w[46 + i].f52 = (s32)taiiku_kari_crowd_tpg;
        class_w[46 + i].f20 = (*(s16 *)&taiiku_spr_aux[1]);
        class_w[46 + i].f26 = 4096;
        class_w[46 + i].f28 = 4096;
        class_w[46 + i].f07 = 128;
        class_w[46 + i].f42 = (s16)(-48 - (t << 4));
        class_w[46 + i].f22 = (s16)taiiku_get_crowd_x();
        while (class_w[46 + i].f22 == class_w[45 + i].f22) {
            class_w[46 + i].f22 = (s16)taiiku_get_crowd_x();
        }
        class_w[46 + i].f24 = 0;
    }
}
