/* @CompileEnv: IDO52_FORCE_WEB_COLOR=37:t0,40:a1,51:a2,53:a3 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x190 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x18c */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: patch_insn_reg_field 0x158:rt:t7:t9 0x164:rt:t9:t8 0x168:rt:t4:t7 0x174:rt:t8:t4 0x174:rs:t7:t9 0x198:rt:t8:t4 0x19c:rt:t9:t8 0x1a4:rt:t4:t7 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolStart: 0x80136640 */
/* @SymbolSize: 628 */
/* Address: 0x80136648, Size: 620 bytes (155 instructions), + absorbed */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_set();
extern void sub_sub_step_up();
extern s32 option_panels_scroll();

extern u8 step_mode;
extern s8 menu_csr;
extern u8 voice_level;
typedef struct {
    u8  pad0[3];
    u8  flags;
    u8  pad4[20];
    s16 y;
    u8  pad26[12];
    s16 w;
    u8  pad40[2];
    s16 h;
    u8  pad44[24];
} OptRec;
extern OptRec cw96_active[];
extern CLASS_W class_w[];

s32 option_voice_init(void)
{
    s32 pad;
    s32 i;
    u8 st;
    s16 sp120[6];
    s16 sp108[6];
    s16 sp96[6];
    s16 sp84[6];
    s32 t0;
    s32 a1;

    st = step_mode;
    if (st == 0) {
        class_w[106].frame = 5;
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

            t0 = 46;
            a1 = 24;
            for (i = 0; i < 3; i++) {
                sp120[i] = 0;
                sp108[i] = (s16)a1;
                sp96[i] = 128;
                sp84[i] = 16;
                cw96_active[i + 11].y = (s16)t0;
                cw96_active[i + 11].flags |= 0x80;
                cw96_active[i + 11].h = sp108[i];
                cw96_active[i + 11].w = sp120[i];
                t0 += 2;
                a1 += 24;
            }

            menu_set(1, 3, sp120, sp108, sp96, sp84);

            switch ((u32)voice_level % 3U) {
            case 1:
                class_w[107].frame = (s16)(class_w[107].frame + 1);
                menu_csr = 0;
                break;
            case 0:
                class_w[108].frame = (s16)(class_w[108].frame + 1);
                menu_csr = 1;
                break;
            case 2:
            default:
                class_w[109].frame = (s16)(class_w[109].frame + 1);
                menu_csr = 2;
                break;
            }

            sub_sub_step_up();
        }
    }

    return option_panels_scroll();
}
