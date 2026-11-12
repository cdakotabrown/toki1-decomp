/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=50:t0,52:a2,54:a3 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1e8 */
/* @PostProc: swap_instructions --offset1 0x264 --offset2 0x268 */
/* @PostProc: swap_branch_operands --offsets 0x20c,0x308 */
/* @PostProc: patch_insn_reg_field 0x15c:rt:t9:t6 0x168:rt:t6:t5 0x16c:rt:t7:t9 0x17c:rs:t9:t6 0x17c:rt:t5:t7 0x1a0:rt:t5:t7 0x1a4:rt:t6:t5 0x1ac:rt:t7:t9 */
/* @SymbolStart: 0x801344E4 */
/* @SymbolSize: 800 */
/* Address: 0x801344EC, Size: 792 bytes (198 instructions), + absorbed */
/* `@SymbolSize: 800` deliberately EXCLUDES the 12 bytes at 0x80134804: the */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_set();
extern void sub_sub_step_up();
extern void option_panels_scroll();

extern u8 step_mode;
extern u8 click_mode;
extern u8 mouse_btn_mode;
extern s16 konami_mark_cw_frame;
typedef struct {
    u8 f0, f1, f2, f3;
    u8 pad4[20];
    s16 f24;
    u8 pad26[12];
    s16 f38;
    s16 f40;
    s16 f42;
    u8 pad44[24];
} R68;
extern R68 cw96_active[];
extern R68 D_80128B30_b[];
extern R68 D_80128B30_c[];
extern CLASS_W class_w[];

void option_mouse_init(void)
{
    s32 i;
    R68 *rp;
    s32 f5;
    s16 sp100[4];
    s16 sp92[4];
    s16 sp84[4];
    s16 sp76[4];

    if (step_mode == 0) {
        class_w[106].frame = 3;
        class_w[106].x.half[1] = -160;
        class_w[106].y.half[1] = -120;
        class_w[106].disp_flag = 0;
        class_w[106].scale_y = 32767;
        step_mode = 1;
        class_w[106].scale_x = 32767;
    } else {
        class_w[106].scale_x = (s16)((class_w[106].scale_x / 17) * 16);
        class_w[106].scale_y = (s16)((class_w[106].scale_y / 17) * 16);

        if (class_w[106].scale_x < 4097) {
            class_w[106].scale_y = 4096;
            class_w[106].scale_x = 4096;

            for (i = 0; &cw96_active[i] < &cw96_active[3]; i++) {
                rp = &cw96_active[i + 11];
                sp100[i] = -120;
                sp92[i] = i * 40 - 32;
                rp->f24 = i * 2 + 28;
                sp84[i] = 108;
                sp76[i] = 16;
                rp->f3 |= 0x80;
                rp->f42 = sp92[i];
                rp->f38 = sp100[i];
            }

            menu_set(1, 3, sp100, sp92, sp84, sp76);

            rp = D_80128B30_b;
            i = 0;
            f5 = mouse_btn_mode;
            do {
                rp[14].f24 = i * 2 + 34;
                rp[14].f3 |= 0x80;
                rp[14].f38 = 0;
                rp[14].f42 = i * 16 - 40;
                if (f5 == 1) {
                    class_w[14].frame = (s16)(class_w[14].frame + 1);
                } else {
                    konami_mark_cw_frame = (s16)(konami_mark_cw_frame + 1);
                }
                rp++;
                i++;
            } while (rp < &D_80128B30_b[2]);

            rp = D_80128B30_c;
            i = 0;
            f5 = click_mode;
            do {
                rp[16].f24 = i * 2 + 38;
                rp[16].f3 |= 0x80;
                rp[16].f38 = 0;
                rp[16].f42 = i * 16;
                if (f5 == 0) {
                    class_w[16].frame = (s16)(class_w[16].frame + 1);
                } else {
                    class_w[17].frame = (s16)(class_w[17].frame + 1);
                }
                rp++;
                i++;
            } while (rp < &D_80128B30_c[2]);

            for (i = 0; &cw96_active[i] != &cw96_active[2]; i++) {
                cw96_active[i + 18].f24 = i * 2 + 42;
                cw96_active[i + 18].f42 = i * 16 + 40;
                cw96_active[i + 18].f3 |= 0x80;
                cw96_active[i + 18].f38 = 0;
            }

            sub_sub_step_up();
        }
    }

    option_panels_scroll();
}
