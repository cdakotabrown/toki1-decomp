/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic0 */
/* @CompileEnv: IDO52_LDA_NOINREG_CLASSES=1 IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: remap_stack_slots --map '136:132,96:92' */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x178 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a0 */
/* @PostProc: patch_insn_reg_field 0x108:rt:v0:t0 0x110:rs:v0:t0 0x110:rt:v0:t0 0x114:rt:v1:t7 0x118:rt:a0:t6 0x144:rt:v0:t0 0x148:rt:v1:t7 0x150:rt:a0:t6 0x16c:rt:t0:t4 0x184:rt:t0:t4 0x1c8:rt:t7:t3 0x1d0:rt:t7:t3 0x1d8:rt:t6:t5 0x1dc:rt:t6:t5 0x1e4:rt:t4:t2 0x1e8:rt:t4:t2 0x1f0:rt:t3:t1 0x1f4:rt:t3:t1 0x1fc:rt:t5:t9 0x200:rt:t5:t9 0x208:rt:t2:t8 0x20c:rt:t2:t8 0x214:rt:t1:t0 0x218:rt:t1:t0 0x220:rt:t9:t7 0x224:rt:t9:t7 0x22c:rt:t8:t6 0x230:rt:t8:t6 0x238:rt:t0:t4 0x23c:rt:t0:t4 0x244:rt:t7:t3 0x248:rt:t7:t3 0x250:rt:t6:t5 0x254:rt:t6:t5 0x25c:rt:t4:t2 0x260:rt:t4:t2 0x268:rt:t3:t1 0x26c:rt:t3:t1 */
/* Address: 0x80135974  Size: 672 bytes (168 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void class_clear_n(s32 a0);

typedef struct {
    s32 w[10];
} Blk40;

extern Blk40 taiiku_runner_init_tbl0;
extern Blk40 taiiku_runner_init_tbl1;
extern u8  class_w[];
extern s32 taiiku_kari_runner_tpg;
extern s32 taiiku_spr_img_2;
extern s32 taiiku_spr_anim_2;

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s32 taiiku_spr_aux[];

void taiiku_runner_init(void) {
    Blk40 buf1;
    Blk40 buf2;
    s16 *b1;
    s16 *b2;
    u8 *p;
    s32 i;
    s32 v0;
    s32 t4;
    s32 j;
    s32 *ptr;

    buf1 = taiiku_runner_init_tbl0;
    buf2 = taiiku_runner_init_tbl1;

    ptr = &taiiku_kari_runner_tpg;
    p = class_w;
    b1 = (s16 *)&buf1;
    b2 = (s16 *)&buf2;
    i = 0;
    do {
        class_clear_n(i + 96);
        *(s32 *)(p + 6584) = 0x01000000;
        *(u8 *)(p + 6535) = 128;
        *(s32 *)(p + 6540) = taiiku_spr_img_2;
        *(s32 *)(p + 6544) = taiiku_spr_anim_2;
        *(s32 *)(p + 6580) = (s32)ptr;
        v0 = (*(s16 *)&taiiku_spr_aux[2]);
        t4 = *b1;
        j = *b2;
        i++;
        p += 68;
        b1++;
        b2++;
        *(u8 *)(p + 6464) = 0;
        *(u8 *)(p + 6465) = 128;
        *(s16 *)(p + 6486) = 4096;
        *(s16 *)(p + 6488) = 4096;
        *(u8 *)(p + 6466) = 2;
        *(s16 *)(p + 6480) = (s16)v0;
        *(s16 *)(p + 6502) = (s16)t4;
        *(s16 *)(p + 6484) = (s16)j;
    } while (i < 20);

    p = class_w;
    i = 0;
    v0 = -160;
    do {
        *(s16 *)(p + 6566) = (s16)v0;
        v0 += 8;
        p += 68;
        *(u8 *)(p + 6461) = (u8)(i + 4);
        *(u8 *)(p + 6462) = 64;
        *(u8 *)(p + 6463) = 164;
        i++;
    } while (i < 4);

    p = class_w;
    do {
        p += 68;
        *(u8 *)(p + 6733) = 8;
        *(s16 *)(p + 6770) = 232;
        *(u8 *)(p + 6734) = 64;
        *(u8 *)(p + 6735) = 164;
    } while (p < class_w + 272);

    ((CLASS_W *)class_w)[104].prio = 3;
    ((CLASS_W *)class_w)[105].prio = 2;
    ((CLASS_W *)class_w)[106].prio = 1;
    ((CLASS_W *)class_w)[107].prio = 4;
    ((CLASS_W *)class_w)[108].prio = 5;
    ((CLASS_W *)class_w)[109].prio = 6;
    ((CLASS_W *)class_w)[110].prio = 7;
    ((CLASS_W *)class_w)[111].prio = 9;
    ((CLASS_W *)class_w)[112].prio = 8;
    ((CLASS_W *)class_w)[113].prio = 9;
    ((CLASS_W *)class_w)[114].prio = 9;
    ((CLASS_W *)class_w)[114].x.half[1] = -48;
    ((CLASS_W *)class_w)[114].anim_flag = 64;
    ((CLASS_W *)class_w)[114].disp_flag = 164;
    cursor_cw_x = 0;
    cursor_cw_y = 0;
}
