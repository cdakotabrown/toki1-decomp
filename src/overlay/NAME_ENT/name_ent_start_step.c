/* @SymbolStart: 0x80146014 */
/* @RodataBase: 0x8014C974 */
/* Address: 0x8014601C */
/* Size: 424 bytes (106 insns) */
/* @SymbolSize: 432 */
/* @PostProc: patch_insn_reg_field 0xa0:rt:v1:a1 0xa4:rs:v1:a1 0xa4:rt:v1:a1 0xac:rs:v1:a1 0xb4:rs:v1:a1 0xb4:rt:v0:v1 0xbc:rt:v0:v1 0xc4:rt:v0:v1 0xcc:rt:v0:v1 0xd4:rs:v0:v1 0xfc:rt:t0:t1 0x100:rs:t0:t1 0x100:rt:t0:t1 0x108:rs:t0:t1 0x108:rt:t1:t2 0x110:rt:t1:t2 0x11c:rt:t2:t3 0x120:rs:t2:t3 0x130:rt:t3:t4 0x134:rs:t3:t4 0x134:rt:t3:t4 0x13c:rs:t3:t4 0x13c:rt:t4:t5 0x144:rt:t4:t5 0x150:rt:t5:t6 0x154:rs:t5:t6 0x170:rt:t6:t7 0x174:rs:t6:t7 0x174:rt:t6:t7 0x17c:rs:t6:t7 */
#include "types.h"

extern void set_dec_bri(s32);
extern void name_ent_serifu_set(void);
extern void name_ent_bg_set(s32, s32);
extern void name_ent_text_scroll(void);
extern void cd_read_start(s32, s32, s32);
extern s32  get_CD_State2(void);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void dec_bg_reset(void);
extern void sub_step_up(void);
extern void dec_bg_show(s32);

extern u8  name_ent_serifu_no;
extern u8  bg0_r;
extern u8  bg0_g;
extern u8  bg0_b;
extern u8  step_mode;
extern s32 dec_bg_handle;
extern u8 step_mode;

void name_ent_start_step(void) {
    u32 b;
    s32 n;

    switch (step_mode) {
    case 0:
        name_ent_serifu_no = 3;
        bg0_g = 0;
        bg0_b = 0;
        bg0_r = 0;
        set_dec_bri(0);
        name_ent_serifu_set();
        name_ent_bg_set(0x801A4000, 14);
        step_mode++;
        break;
    case 1:
        b = bg0_r;
        if (b < 129) {
            b = b + 4;
            bg0_g = b;
            bg0_b = b;
            bg0_r = b;
            set_dec_bri(b & 0xFF);
        }
        name_ent_text_scroll();
        break;
    case 2:
        cd_read_start(49, 0x801A4000, 0xAA23);
        step_mode++;
        break;
    case 3:
        if (get_CD_State2() & 1) {
            value_cache_find_or_load(0xACE6, 1);
            step_mode++;
        }
        break;
    case 4:
        if (get_CD_State2() & 1) {
            n = dec_bg_init(1);
            indexed_byte_lsb_set(1, n);
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                value_cache_find_or_load(0xACE6, 1);
            } else {
                sub_step_up();
            }
        }
        break;
    }
    dec_bg_show(1);
}
