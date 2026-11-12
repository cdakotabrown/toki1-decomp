/* @SymbolStart: 0x80141454 */
/* @SymbolSize: 316 */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x108 */
/* @PostProc: patch_insn_reg_field 0x44:rt:a1:a0 0x6c:rt:t6:v1 0x70:rs:t6:v1 0x70:rt:t6:v1 0x78:rs:t6:v1 0x78:rt:t7:t6 0x80:rt:t7:t6 0x8c:rt:t8:t7 0x90:rs:t8:t7 0x94:rt:t9:v1 0x98:rs:t9:v1 0x98:rt:t9:v1 0xa0:rs:t9:v1 0xa0:rt:t0:t8 0xa8:rt:t0:t8 0xbc:rt:t1:t9 0xc0:rt:t1:t9 0xe4:rt:t2:v1 0xe8:rs:t2:v1 0xe8:rt:t2:v1 0xf0:rs:t2:v1 0xf0:rt:t3:t0 0xf8:rt:t3:t0 0xfc:rs:a1:a0 0xfc:rt:a1:a0 0x104:rs:a1:a0 0x104:rt:t5:v0 0x108:rs:a1:a0 0x10c:rt:t4:t1 0x110:rs:t5:v0 0x114:rs:t4:t1 0x120:rt:t4:t1 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6,a1:29 */
/* Address: 0x8014145C; window starts 0x80141454 (absorbed cross-prologue head */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6,a1:29 */
#include "types.h"

extern s32  sub_sub_step_up();
extern void image_rect_move();
extern s32  get_CD_State2();
extern s32  cd_read_start2();
extern void decdct();
extern void set_dec_bri();
extern void dec_bg_reset();
extern s32  dec_bg_show();
extern s32  DrawSync();
extern u8 step_mode;
extern u8 screen_bri;

s32 etc_card_bg_load1(void)
{
    s32 r;
    s32 sel;
    s32 val;
    s32 newval;

    sel = step_mode;
    r = sel;
    switch (sel) {
    case 0:
        dec_bg_reset();
        r = cd_read_start2(9, 0x80162000, 0xA1A2);
        step_mode++;
        break;
    case 1:
        r = get_CD_State2();
        if (r & 1) {
            step_mode++;
        }
        break;
    case 2:
        decdct(704, 0, 320, 240);
        image_rect_move(704, 32, 320, 208, 704, 0);
        r = DrawSync(0);
        step_mode++;
        break;
    case 3:
        val = screen_bri;
        newval = (val + 1) & 0xFF;
        if (((u32)val < 128) != 0) {
            screen_bri = newval;
            set_dec_bri(newval);
            r = dec_bg_show(2);
        } else {
            screen_bri = newval;
            r = sub_sub_step_up(newval);
        }
        break;
    }
    return r;
}
