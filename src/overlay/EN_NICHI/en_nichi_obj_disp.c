/* en_nichi_obj_disp - EN_NICHI overlay, Address: 0x80133E24, Size: 600 bytes */
/* @RodataBase: 0x8013949C */
/* @PostProc: patch_insn_reg_field 0x74:rt:t0:t2 0xa8:rt:t1:t0 0xac:rt:t0:t2 0xb0:rs:t1:t0 0xb0:rt:t2:t1 0xb4:rt:t2:t1 0x178:rt:t7:t6 0x17c:rt:t6:t1 0x180:rs:t7:t6 0x180:rt:t8:t7 0x184:rd:t9:t8 0x184:rt:t8:t7 0x188:rs:t9:t8 0x188:rt:t1:t9 0x194:rt:t6:t1 0x198:rt:t1:t9 0x200:rt:t0:t1 0x204:rt:t1:t5 0x208:rs:t0:t1 0x208:rt:t2:t0 0x20c:rd:t3:t2 0x20c:rt:t2:t0 0x210:rs:t3:t2 0x210:rt:t5:t3 0x21c:rt:t1:t5 0x220:rt:t5:t3 */
#include "types.h"

typedef struct {
    s32 kind;
    s32 u04;
    s32 u08;
    s32 u0C;
    s32 u10;
    s16 xl;
    s16 xh;
    s16 yl;
    s16 yh;
    s32 ang;
    s32 spd;
    s32 tmr;
    s32 sel;
    s32 inp;
} EnNichiObj;

typedef struct {
    u8 u00;
    u8 u01;
    u8 u02;
    u8 u03;
    u8 pad04[20];
    s16 u18;
    u8 pad1A[12];
    s16 u26;
    u8 pad28[2];
    s16 u2A;
    u8 pad2C[23];
    u8 u43;
} EnNichiSpr;

extern EnNichiObj en_nichi_obj_wk[];
extern EnNichiSpr class_w[];
extern s16 en_nichi_obj_pat_tbl[][6];
extern u8 en_nichi_kind_attr_tbl[];

extern void en_nichi_obj_sprite_set(s32, s32);
extern s32 en_nichi_obj_octant_get(s32);

void en_nichi_obj_disp(void)
{
    s32 i;
    s32 f;
    s32 pat;

    for (i = 0; i != 10; i++) {
        if (en_nichi_obj_wk[i].kind & 0x80) {
            class_w[i + 128].u01 = 3;
            en_nichi_obj_wk[i].u10 += 4;
            en_nichi_obj_sprite_set(i, 1);
        } else {
            class_w[i + 128].u01 = 7;
            en_nichi_obj_wk[i].u10 += 1;
            en_nichi_obj_sprite_set(i, 0);
        }
        if (en_nichi_obj_wk[i].u10 >= 15) {
            en_nichi_obj_wk[i].u0C += 1;
            en_nichi_obj_wk[i].u10 = 0;
        }

        switch ((u32)(en_nichi_obj_wk[i].kind & 0x7F)) {
        case 0:
        default:
            class_w[i + 128].u03 = 0;
            break;
        case 1:
        case 2:
        case 3:
            if (en_nichi_obj_wk[i].u0C >= 6) {
                en_nichi_obj_wk[i].u0C = 0;
            }
            f = en_nichi_obj_octant_get(i);
            pat = en_nichi_obj_pat_tbl[f][en_nichi_obj_wk[i].u0C];
            if (pat & 0x80) {
                class_w[i + 128].u03 = 133;
            } else {
                class_w[i + 128].u03 = 132;
            }
            class_w[i + 128].u18 = (s16)(pat & 0x7F);
            class_w[i + 128].u26 = en_nichi_obj_wk[i].xh;
            class_w[i + 128].u43 = en_nichi_kind_attr_tbl[en_nichi_obj_wk[i].kind & 0x7F];
            class_w[i + 128].u2A = en_nichi_obj_wk[i].yh;
            break;
        case 4:
            if (en_nichi_obj_wk[i].u0C >= 4) {
                en_nichi_obj_wk[i].u0C = 0;
            }
            f = en_nichi_obj_octant_get(i);
            pat = en_nichi_obj_pat_tbl[f + 8][en_nichi_obj_wk[i].u0C];
            if (pat & 0x80) {
                class_w[i + 128].u03 = 133;
            } else {
                class_w[i + 128].u03 = 132;
            }
            class_w[i + 128].u18 = (s16)(pat & 0x7F);
            class_w[i + 128].u26 = en_nichi_obj_wk[i].xh;
            class_w[i + 128].u43 = en_nichi_kind_attr_tbl[en_nichi_obj_wk[i].kind & 0x7F];
            class_w[i + 128].u2A = en_nichi_obj_wk[i].yh;
            break;
        }
    }
}
