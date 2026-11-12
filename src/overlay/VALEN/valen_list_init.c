/* @SymbolStart: 0x80132834 */
/* @SymbolSize: 1240 */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_COPYPROP_SITE=lod:2:deny IDO52_FORCE_WEB_RANGE_SPLIT=43,40,37,28,31 IDO52_SPLIT_BLOCKS=43#1:3,43.1:7+8+9,40#1:3,40.1:8,40.2:11+12,37#1:3,37.1:8+9,37.2:12+13,37.3:15+16,28#1:3,28.1:10,28.2:14,31#1:3 IDO52_FORCE_WEB_NOSPILL=43.0:5,43.1/-1:5,40.0:5,40.1/-1:5,40.2/-1:5,37.0:5,37.1/-1:5,37.2/-1:5,37.3/-1:5,28.0:5,28.1/-1:5,28.2/-1:5,31.0:5 IDO52_FORCE_WEB_COLOR=43:a0,40:v1,189:v1,37:a3,190:a3,195:a3,199:a3,28:a2,192:a2,197:a2,46:s1,68:a1 IDO52_CMOT_KILL_AVAIL=166:9,167:9,168:9,166:13,167:13,168:13 IDO52_UGEN_FORCE_TARGET_SAFE=17,26,43 IDO52_WEB_PIECE_RESCUE=28.2 IDO52_AS1_TIE_TAKE=149,217,259,543,326,357,359,461 IDO52_AS1_TIE_REJECT=151,168,746,670,674,677 IDO52_UGEN_TEMP_REG=2:t8,3:t9,4:t0,5:t1,6:t4,70:t3,7:t5,8:t6,10:t8,11:t9,12:t0,26:t7,27:t8,28:t9,16:t5,31:t2,32:t3,29:t0,39:t1,15:t4,41:t3,34:t7,38:t4,40:t2,33:t6,35:t8,36:t9,37:t0,42:t4,13:t1,14:t3,18:t7,23:t3,24:t4,20:t9,22:t2,19:t8,21:t0 IDO52_AS1_XBB_SKIP_CAND=3,5,7,9 IDO52_AS1_FTF_BACKWARD=occ:0 */
/* @PostProc: patch_insn_reg_field 0x11c:rd:a1:t2 0x164:rt:a1:t2 */
/* valen_list_init - VALEN overlay, Address: 0x8013283C, 1232 bytes (308 insns) */
/* `@PostProc: pad_trailing_nop --count 19` faking the instruction count on a */
#include "types.h"
extern s16 valen_list_no;
extern u32 get_h_tokimeki();
extern u32 get_h_yuukou();
extern void valen_list_shuffle();
extern void sub_sub_step_up();
extern u8  screen_w[];
extern s16 girl0_tokimeki;
extern s16 girl9_tokimeki;
extern s16 girl9_yuukou;
extern s32 girl9_flag;
extern u32 frame_counter;
extern s16 valen_girl_list[];
extern s16 valen_girl_list_cnt;
u8 valen_top3_tbl[3] = {0,0,0};
extern u8  valen_girl9_flag;
extern u8  valen_basyo_mode;

typedef struct { int a:1; int f:1; int r:30; } Bits;
typedef struct { unsigned a:1; unsigned f:1; unsigned r:30; } UBits;
void valen_list_init()
{
    s16 sortList[14];
    s16 i;
    s16 count;
    s16 rem;
    s16 best2;
    s16 best1;
    s16 best0;
    u8 *p;
    int tmp;

    valen_list_no = 0;
    valen_basyo_mode = 0;
    valen_girl_list_cnt = 0;
    valen_girl9_flag = 0;
    count = 0;
    if ((girl9_flag << 30) >= 0) {
        if (girl9_tokimeki >= 20) {
            valen_girl9_flag = 1;
            girl9_yuukou = 50;
        }
    }
    valen_top3_tbl[1] = valen_top3_tbl[2] = 255;
    valen_top3_tbl[0] = valen_top3_tbl[1];
    best1 = best2 = 0;
    best0 = best1;
    for (i = 0; i < 11; i++) {
        p = &screen_w[i * 56];
        if (((Bits *)(p + 456))->f) {
            if (get_h_tokimeki(i) >= 70) {
                if (best0 < *(s16 *)(p + 450) + *(s16 *)(p + 446)) {
                    best2 = best1;
                    goto L1;
                L1:
                    tmp = best0;
                    best0 = *(s16 *)(p + 446) + *(s16 *)(p + 450);
                    best1 = tmp;
                    goto L3;
                L3:
                    valen_top3_tbl[2] = valen_top3_tbl[1];
                    valen_top3_tbl[1] = valen_top3_tbl[0];
                    valen_top3_tbl[0] = i;
                } else if (best1 < *(s16 *)(p + 450) + *(s16 *)(p + 446)) {
                    best2 = best1;
                    goto L2;
                L2:
                    best1 = *(s16 *)(p + 446) + *(s16 *)(p + 450);
                    goto L4;
                L4:
                    valen_top3_tbl[2] = valen_top3_tbl[1];
                    valen_top3_tbl[1] = i;
                } else if (best2 < *(s16 *)(p + 450) + *(s16 *)(p + 446)) {
                    best2 = *(s16 *)(p + 450) + *(s16 *)(p + 446);
                    valen_top3_tbl[2] = i;
                }
            }
        }
    }

    for (i = 0; i < 11; i++) {
        p = &screen_w[i * 56];
        if (((UBits *)(p + 456))->f ||
            (i == 9 && valen_girl9_flag == 1)) {
            if (get_h_yuukou(i) >= 50 ||
                (valen_girl9_flag == 1 && i == 9)) {
                sortList[count] = i;
                valen_girl_list_cnt = valen_girl_list_cnt + 1;
                p[457] = (p[457] & 0xFFFE) | 1;
                count++;
            }
        }
    }

    if (valen_girl_list_cnt > 0) {
        rem = frame_counter % valen_girl_list_cnt;
        p = (u8 *)valen_girl_list;
        for (i = 0; i < valen_girl_list_cnt; i++) {
            ((s16 *)p)[i] = sortList[(i + rem) % valen_girl_list_cnt];
        }
        valen_list_shuffle();
    }

    for (i = 0; i < valen_girl_list_cnt; i++) {
        if (valen_girl_list[i] == 0 && girl0_tokimeki >= 70) {
            valen_girl_list[i] = valen_girl_list[valen_girl_list_cnt - 1];
            valen_girl_list[valen_girl_list_cnt - 1] = 0;
        } else if (valen_girl_list[i] == 0 && valen_girl9_flag != 0) {
            valen_girl_list[i] = valen_girl_list[valen_girl_list_cnt - 1];
            valen_girl_list[valen_girl_list_cnt - 1] = 0;
        } else if (valen_girl_list[i] == 0) {
            valen_girl_list[i] = valen_girl_list[0];
            valen_girl_list[0] = 0;
        }
        if (9 == valen_girl_list[i] && valen_girl9_flag != 0) {
            valen_girl_list[i] = valen_girl_list[0];
            valen_girl_list[0] = 9;
        }
    }
    sub_sub_step_up();
}
