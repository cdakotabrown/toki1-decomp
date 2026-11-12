/* Function: text_sys_init */
/* Address: 0x8004E9BC */
/* Size: 452 bytes (113 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x180 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: patch_insn_reg_field 0x1a8:rt:t6:v0 0x1b0:rt:t6:v0 */
/* carried `@PostProc: strip_instructions --remove 0x184,0x18c,0x194`, which is */
/* `@CompileFlags: -Wc,-pic1`: the +16 IS the PIC linkage area, and the frame */
#include "types.h"

typedef struct {
    s16 field_00;
    s16 field_02;
    u8 field_04;
    u8 field_05;
    u8 field_06;
    u8 field_07;
} TextEntry;

extern u8 text_sys_init_tbl1[32];
extern u8 text_sys_init_tbl2[32];
extern u8 text_sys_init_tbl3[32];
extern u8 text_sys_init_tbl4[32];
extern u8 text_sys_init_tbl0[32];
extern TextEntry disp_string_tbl0[];

struct {
    s16 f_460;
    s16 f_462;
    s16 f_464;
    s16 f_466;
    s16 f_468;
} disp_string_state = { 0 };
extern u8 k_cell_w;

extern void load_image_rect(s32, s32, s32, s32, u8 *);
extern void vram_rect_clear(s32);
extern void ClearImage(RECT *, s32, s32, s32);

void text_sys_init(void) {
    s32 i;
    RECT rect;

    load_image_rect(0x100, 0x1F8, 2, 8, text_sys_init_tbl1);
    load_image_rect(0x120, 0x1F8, 2, 8, text_sys_init_tbl2);
    load_image_rect(0x110, 0x1F8, 2, 8, text_sys_init_tbl3);
    load_image_rect(0x130, 0x1F8, 2, 8, text_sys_init_tbl4);
    load_image_rect(0x320, 0x1FF, 0x20, 1, text_sys_init_tbl0);
    load_image_rect(0x360, 0x1FF, 0x20, 1, text_sys_init_tbl0);
    vram_rect_clear(2);

    rect.x = 0x102;
    rect.y = 0x1F8;
    rect.w = 2;
    rect.h = 8;
    ClearImage(&rect, 0x80, 0x80, 0x80);

    for (i = 0; i < 52; i++) {
        disp_string_tbl0[i].field_00 = 0;
        disp_string_tbl0[i].field_02 = 0;
        disp_string_tbl0[i].field_04 = 0;
        disp_string_tbl0[i].field_05 = 0;
        disp_string_tbl0[i].field_06 = 0;
        disp_string_tbl0[i].field_07 = 0;
    }

    disp_string_state.f_462 = 0;
    disp_string_state.f_460 = 0;
    disp_string_state.f_468 = 0;
    disp_string_state.f_464 = 0;

    if (k_cell_w != 14 && k_cell_w != 16) {
        k_cell_w = 14;
    }
}
