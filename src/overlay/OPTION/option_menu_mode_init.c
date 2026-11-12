/* @SymbolStart: 0x8013554C */
/* @SymbolSize: 1072 */
/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1b0 */
/* @PostProc: patch_insn_reg_field 0x170:rt:t7:t9 0x17c:rt:t9:t8 0x180:rt:t5:t7 0x190:rs:t7:t9 0x190:rt:t8:t5 0x1b4:rt:t8:t5 0x1b8:rt:t9:t8 0x1c0:rt:t5:t7 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1,44:t0 */
/* Address: 0x80135554, Size: 1064 bytes (266 instructions) */
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

extern void sub_sub_step_up(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_switch(s32, s32);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void option_panels_scroll(void);

extern Rec68 cw96_active[];


extern u8 menu_mode;
extern s8 menu_csr;
extern u8 step_mode;

extern u8 option_menu_mode_init_str0[];
extern u8 option_menu_mode_init_str1[];
extern u8 option_menu_mode_init_str2[];
extern u8 option_menu_mode_init_str3[];
extern u8 option_menu_mode_init_str4[];
extern u8 option_menu_mode_init_str5[];
extern u8 option_menu_mode_init_str6[];
extern u8 option_menu_mode_init_str7[];
extern u8 option_menu_mode_init_str8[];
extern u8 option_menu_mode_init_str9[];
extern u8 option_menu_mode_init_str10[];
extern u8 option_menu_mode_init_str11[];
extern CLASS_W class_w[];

void option_menu_mode_init(void)
{
    s16 arrE[6];
    s16 arrD[6];
    s16 arrC[6];
    s16 arrB[6];
    s16 arrA[6];
    s32 i;
    s32 div;
    s32 scale;
    s32 pat;
    s32 ofs;
    u32 step;

    step = step_mode;
    switch (step) {
    case 0:
        class_w[106].frame = 2;
        class_w[106].x.half[1] = -160;
        class_w[106].y.half[1] = -120;
        class_w[106].disp_flag = 0;
        class_w[106].scale_y = 32767;
        step_mode = 1;
        class_w[106].scale_x = 32767;
        break;
    case 1:
        div = 17;
        class_w[106].scale_x = class_w[106].scale_x / div * 16;
        class_w[106].scale_y = class_w[106].scale_y / div * 16;

        if (class_w[106].scale_x <= 4096) {
            class_w[106].scale_y = 4096;
            class_w[106].scale_x = 4096;

            pat = 123;
            ofs = -56;
            for (i = 0; i < 4; i++) {
                arrD[i] = -32;
                arrC[i] = (s16)ofs;
                arrB[i] = 160;
                arrA[i] = 16;
                cw96_active[11 + i].f24 = (s16)pat;
                pat += 2;
                ofs += 24;
                cw96_active[11 + i].flag3 |= 0x80;
                cw96_active[11 + i].f42 = arrC[i];
                cw96_active[11 + i].f38 = arrD[i];
            }

            menu_set(1, 4, arrD, arrC, arrB, arrA);
            k_sub_reset();

            switch (menu_mode & 3) {
            case 0:
                class_w[107].frame--;
                menu_csr = 0;
                break;
            case 1:
                class_w[108].frame--;
                menu_csr = 1;
                break;
            case 3:
                class_w[109].frame--;
                menu_csr = 2;
                break;
            case 2:
                class_w[110].frame--;
                menu_csr = 3;
                break;
            }

            set_kanji_string(-128, 64, 15, (s32)option_menu_mode_init_str0, 0);
            set_kanji_string(-128, 80, 15, (s32)option_menu_mode_init_str1, 0);
            set_kanji_string(-128, 96, 15, (s32)option_menu_mode_init_str2, 0);
            set_kanji_string(-128, 64, 15, (s32)option_menu_mode_init_str3, 0);
            set_kanji_string(-128, 80, 15, (s32)option_menu_mode_init_str4, 0);
            set_kanji_string(-128, 96, 15, (s32)option_menu_mode_init_str5, 0);
            set_kanji_string(-128, 64, 15, (s32)option_menu_mode_init_str6, 0);
            set_kanji_string(-128, 80, 15, (s32)option_menu_mode_init_str7, 0);
            set_kanji_string(-128, 96, 15, (s32)option_menu_mode_init_str8, 0);
            set_kanji_string(-128, 64, 15, (s32)option_menu_mode_init_str9, 0);
            set_kanji_string(-128, 80, 15, (s32)option_menu_mode_init_str10, 0);
            set_kanji_string(-128, 96, 15, (s32)option_menu_mode_init_str11, 0);

            k_sub_disp_start(12);

            for (i = 0; i < 12; i++) {
                k_disp_switch(i + 8, 0);
            }

            sub_sub_step_up();
        }
        break;
    }

    option_panels_scroll();
}
