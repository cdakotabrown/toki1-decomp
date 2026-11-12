/* @SymbolStart: 0x8014A77C */
/* name_ent_bg_load11 - NAME_ENT overlay, Address: 0x8014A784, Size: 272 bytes (68 insns) */
/* @SymbolSize: 280 */
/* @PostProc: swap_registers a0:v1 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x3c:rs:v0:v1 0x5c:rt:t7:v1 0x60:rs:t7:v1 0x60:rt:t7:v1 0x68:rs:t7:v1 0x68:rt:t8:t7 0x70:rt:t8:t7 0x8c:rt:t9:v1 0x90:rs:t9:v1 0x90:rt:t9:v1 0x98:rs:t9:v1 0x98:rt:t0:t8 0xa0:rt:t0:t8 0xac:rt:t1:t9 0xb0:rs:t1:t9 0xbc:rt:v1:a0 0xc0:rt:v1:a0 0xcc:rt:t2:t0 0xd0:rs:t2:t0 0xd0:rt:t2:t0 0xd8:rs:t2:t0 0xdc:rt:v1:a0 0xe8:rt:v1:a0 0x114:rt:v1:a0 0x34:rt:v1:a0 */
#include "types.h"

extern void name_ent_serifu_set(void);
extern void name_ent_text_scroll(void);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32 a0);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void dec_bg_reset(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void sub_step_up(void);
extern void dec_bg_show(s32 a0);

extern u8 name_ent_serifu_no;
extern u8 step_mode;
extern s32 dec_bg_handle;

void name_ent_bg_load11(void)
{
    s32 buttons;
    s32 result;

    switch (step_mode) {
    case 0:
        name_ent_serifu_no = 37;
        name_ent_serifu_set();
        step_mode = step_mode + 1;
        break;
    case 1:
        name_ent_text_scroll();
        break;
    case 2:
        value_cache_find_or_load(18803, 1);
        step_mode = step_mode + 1;
        break;
    case 3:
        buttons = get_CD_State2();
        if (buttons & 0x0001) {
            result = dec_bg_init(1);
            indexed_byte_lsb_set(1, result);
            if (dec_bg_handle == -1) {
                dec_bg_reset();
                value_cache_find_or_load(18803, 1);
            } else {
                cd_read_start2(65, 0x80180000, 0xAC50);
                sub_step_up();
            }
        }
        break;
    }
    dec_bg_show(1);
}
