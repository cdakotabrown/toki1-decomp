/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=occ:8 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x194 */
/* @PostProc: patch_insn_reg_field 0xc8:rt:t0:a3 0xcc:rt:t1:t0 0xd0:rt:a3:t1 0x158:rt:t7:t9 0x164:rt:t9:t8 0x168:rt:t4:t7 0x170:rt:a3:t1 0x174:rs:t7:t9 0x174:rt:t8:t4 0x178:rs:v1:t1 0x178:rt:v1:t1 0x180:rs:t0:v1 0x180:rt:t0:v1 0x184:rs:t1:a0 0x184:rt:t1:a0 0x18c:rs:a0:t0 0x18c:rt:a0:t0 0x190:rs:t0:a3 0x194:rs:t1:t0 0x198:rt:t8:t4 0x19c:rt:t9:t8 0x1a4:rt:t4:t7 */
/* @SymbolStart: 0x80133760 */
/* @SymbolSize: 564 */
/* option_k_speed_init -- OPTION overlay, Address: 0x80133768 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct {
    u8 padA[3];
    u8 flag3;
    s32 padB[5];
    s16 f24;
    s16 padC[6];
    s16 f38;
    s16 padD;
    s16 f42;
    s32 padE[6];
} Rec68;

extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern s32 get_k_speed(void);
extern void k_speed_set(s32);
extern void sub_sub_step_up(void);

extern Rec68 cw96_active[];


extern s8 menu_csr;
extern u8 step_mode;
extern CLASS_W class_w[];

void option_k_speed_init(void)
{
    s32 padTop[4];
    s16 arrD[6];
    s16 arrC[6];
    s16 arrB[6];
    s16 arrA[6];
    s32 i;
    s32 div;
    s32 sel;
    s32 pat;
    s32 ofs;

    if (step_mode == 0) {
        class_w[106].frame = 1;
        class_w[106].x.half[1] = -160;
        class_w[106].y.half[1] = -120;
        class_w[106].disp_flag = 0;
        class_w[106].scale_y = 32767;
        step_mode = 1;
        class_w[106].scale_x = 32767;
    } else {
        div = 17;
        class_w[106].scale_x = class_w[106].scale_x / div * 16;
        class_w[106].scale_y = class_w[106].scale_y / div * 16;

        if (class_w[106].scale_x <= 4096) {
            class_w[106].scale_y = 4096;
            class_w[106].scale_x = 4096;

            ofs = -88;
            pat = 14;
            for (i = 0; i < 3; i++) {
                arrD[i] = 16;
                arrC[i] = (s16)ofs;
                arrB[i] = 40;
                arrA[i] = 16;
                cw96_active[11 + i].f24 = (s16)pat;
                pat += 2;
                ofs += 40;
                cw96_active[11 + i].flag3 |= 0x80;
                cw96_active[11 + i].f42 = arrC[i];
                cw96_active[11 + i].f38 = arrD[i];
            }

            menu_set(1, 3, arrD, arrC, arrB, arrA);

            sel = get_k_speed();
            switch (sel) {
            case 0:
                menu_csr = 0;
                break;
            case 1:
                menu_csr = 1;
                break;
            case 2:
                menu_csr = 2;
                break;
            default:
                k_speed_set(0);
                menu_csr = 0;
                break;
            }

            sub_sub_step_up();
        }
    }
}
