/* @SymbolSize: 1332 */
/* @PostProc: patch_stack_complete --old 280 --new 304 */
/* @PostProc: remap_stack_slots --map '72:84,96:108,120:132,128:140,136:148,144:156,152:164,184:196,216:228,248:260' */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x224 */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x224 */
/* @PostProc: swap_instructions --offset1 0x224 --offset2 0x228 */
/* name_ent_grid_init - NAME_ENT overlay, Address: 0x8013C100, 1332 bytes (333 insns) */
#include "types.h"

typedef struct {
    u8   pad0;
    u8   f1;
    u8   f2;
    u8   f3;
    u8   f4;
    u8   f5;
    u8   pad6;
    u8   f7;
    u8   pad8[4];
    u32 *f12;
    u32 *f16;
    u32  pad20;
    s16  f24;
    s16  pad26[6];
    s16  f38;
    s16  pad40;
    s16  f42;
    u32  pad44[2];
    u32 *f52;
    u32  f56;
    u32  pad60;
    u8   pad64[3];
    u8   f67;
} Rec68;

extern void menu_set(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *a4, s16 *a5);
extern void load_tpage_buf_lock(s32 a0, s32 a1, s32 a2);
extern void palette_upload_enqueue(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void bg_set(u32 *a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5,
                          s32 a6, s32 a7, s32 a8, s32 a9);
extern void class_clear_n(s32 a0);
extern void sub_step_up(void);

extern u8    now_icon;
extern s8    menu_csr;
extern u8    card_sys_present;
extern u32   _sys_default_tpage_set_tbl2[];
extern u32   basyo_init_tbl1[];
extern u32   basyo_init_tbl2[];
extern s32   name_ent_grid_cur;
extern s32   name_ent_view_page;
extern s32   name_ent_grid_cnt;
extern s32   name_ent_grid_len;
extern Rec68 cw96_active[];

void name_ent_grid_init(void)
{
    s16 x14[16];
    s16 y14[16];
    s16 w14[16];
    s16 h14[16];
    s16 x4[4];
    s16 y4[4];
    s16 w4[4];
    s16 h4[4];
    u32 tex0[6];
    u32 tex1[6];
    s32 i;

    for (i = 0; i < 6; i++) {
        tex0[i] = (s32)0x80182000 + i * 0x4000;
        tex1[i] = (s32)0x801A4000 + i * 0x4000;
    }

    for (i = 0; i < 14; i++) {
        x14[i] = (i / 7) * 32 - 144;
        y14[i] = (i % 7) * 16 - 18;
        w14[i] = 32;
        h14[i] = 16;
    }
    menu_set(0, 14, x14, y14, w14, h14);

    for (i = 0; i < 4; i++) {
        x4[i] = (i & 1) * 44 - 144;
        y4[i] = (i & 2) * 8 - 96;
        w4[i] = 40;
        h4[i] = 20;
    }
    menu_set(1, 4, x4, y4, w4, h4);

    now_icon = 0;
    menu_csr = -1;
    name_ent_grid_cur = 0;
    name_ent_view_page = 1;
    name_ent_grid_cnt = 0;
    name_ent_grid_len = 0;

    for (i = 0; i < 45; i++) {
        cw96_active[i].f24 = i;
        cw96_active[i].f2 = 64;
        cw96_active[i].f3 = 36;
        cw96_active[i].f1 = 5;
        cw96_active[i].f56 = 0x01000000;
        cw96_active[i].f12 = (u32 *)0x801C048C;
        cw96_active[i].f16 = (u32 *)0x801C0540;
        cw96_active[i].f52 = (u32 *)0x801C047C;
        cw96_active[i].f4 = 0;
        cw96_active[i].f67 = 0;
        cw96_active[i].f5 = 128;
        cw96_active[i].f38 = -160;
        cw96_active[i].f42 = -120;
        cw96_active[i].f7 = 128;
    }

    for (i = 0; i < 11; i++) {
        cw96_active[45 + i].f1 = 4;
        cw96_active[45 + i].f2 = 64;
        cw96_active[45 + i].f56 = 0x40000000;
        cw96_active[45 + i].f3 = 36;
        cw96_active[45 + i].f12 = basyo_init_tbl1;
        cw96_active[45 + i].f16 = basyo_init_tbl2;
        cw96_active[45 + i].f52 = _sys_default_tpage_set_tbl2;
        cw96_active[45 + i].f4 = 8;
        cw96_active[45 + i].f24 = 89;
        cw96_active[45 + i].f67 = 0;
        cw96_active[45 + i].f5 = 1;
        cw96_active[45 + i].f38 = (i / 4) * 65 - 15;
        cw96_active[45 + i].f42 = (i % 4) * 16 + 36;
    }

    load_tpage_buf_lock(5, cw96_active[0].f52[0], 1);
    load_tpage_buf_lock(6, cw96_active[0].f52[1], 1);
    load_tpage_buf_lock(7, cw96_active[0].f52[2], 1);
    load_tpage_buf_lock(8, cw96_active[0].f52[3], 1);

    cw96_active[18].f3 = 0;
    for (i = 4; i < 19; i++) {
        cw96_active[i].f3 |= 0x80;
    }

    if (card_sys_present != 0) {
        cw96_active[30].f67 = 17;
    } else {
        cw96_active[31].f67 = 17;
    }
    cw96_active[30].f3 |= 0x80;
    cw96_active[31].f3 |= 0x80;

    palette_upload_enqueue(0x801A2000, 16, 1, 1, 0);
    palette_upload_enqueue(0x801A2000, 17, 1, 1, 1);
    palette_upload_enqueue(0x80180000, 31, 1, 1, 0);
    bg_set(tex0, 0, 6, 3, 2, 511, -160, -120, 10, 13);
    bg_set(tex1, 1, 6, 3, 2, 496, -160, -120, 8, 10);
    class_clear_n(114);
    sub_step_up();
}
