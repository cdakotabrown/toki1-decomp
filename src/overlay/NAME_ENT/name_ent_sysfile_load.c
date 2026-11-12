/* @SymbolStart: 0x8013CCD0 */
/* @RodataBase: 0x8014C174 */
/* @PostProc: patch_stack_complete --old 48 --new 64 */
/* @PostProc: remap_stack_slots --from 36 --to 48 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rt:v0:v1 0x4c:rt:v0:v1 0x50:rs:v0:v1 0x50:rt:v0:v1 0x58:rs:v0:v1 0x80:rt:v0:v1 0x84:rs:v0:v1 0x84:rt:v0:v1 0x8c:rs:v0:v1 0xec:rt:v0:v1 0xf0:rs:v0:v1 0xf0:rt:v0:v1 0xf8:rs:v0:v1 0x114:rt:v0:t2 0x118:rs:v0:t2 0x118:rt:v0:t2 0x11c:rt:t2:t3 0x120:rd:t2:t3 0x120:rs:t2:t3 0x120:rt:v0:t2 0x124:rs:t2:t3 0x124:rt:t2:t3 0x12c:rt:t2:t3 0x138:rs:v0:t2 0x14c:rt:t3:t4 0x154:rt:t3:t4 0x1a0:rt:t4:t5 0x1ac:rt:t4:t5 0x208:rt:v0:v1 0x20c:rs:v0:v1 0x20c:rt:v0:v1 0x214:rs:v0:v1 0x214:rt:t5:t6 0x21c:rt:t5:t6 0x220:rt:v1:v0 0x224:rs:v1:v0 0x224:rt:v1:v0 0x228:rs:v0:v1 0x228:rt:t7:t8 0x22c:rs:v1:v0 0x22c:rt:t6:t7 0x230:rs:t6:t7 0x23c:rt:t7:t8 0x300:rt:v0:v1 0x304:rs:v0:v1 0x304:rt:v0:v1 0x30c:rs:v0:v1 0x30c:rt:t8:t9 0x314:rt:t8:t9 0x318:rt:v1:v0 0x31c:rs:v1:v0 0x31c:rt:v1:v0 0x320:rs:v0:v1 0x320:rt:t0:t1 0x324:rs:v1:v0 0x324:rt:t9:t0 0x328:rs:t9:t0 0x32c:rs:v1:v0 0x32c:rt:t1:t3 0x338:rt:t0:t1 0x33c:rs:t1:t3 */
/* @SymbolSize: 900 */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern u8 last_album;
extern s32 pad_trig;
extern s32 card_cnt;
extern s32 name_ent_view_page;
extern u8 album_work[];

extern u8 name_ent_load1_str0[];
extern u8 name_ent_load1_str1[];
extern u8 name_ent_load1_str2[];
extern u8 name_ent_load1_str3[];
extern u8 save_format_main_str6[];
extern u8 save_format_main_str7[];

extern void card_step_init(void);
extern s32 card_file_read_main(void);
extern void card_state_clear(void);
extern void save_flag_clear(void);
extern void goto_sub_step(s32 arg);
extern s32 save_work_restore(s32 arg);
extern void text_sys_init(void);
extern void name_ent_card_io_noop(void);
extern void name_ent_cursor_set(s32 arg);
extern void name_ent_album_info_disp(void);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32 arg);
extern void sub_sub_step_up(void);
extern void set_kanji_string(s32 x, s32 y, s32 w, u8 *str, s32 f);
extern void k_disp_start(s32 arg);
extern s32 get_pad_str(void);
extern s32 get_pad_str2(void);
extern s32 card_file_delete(s32 arg);

void name_ent_sysfile_load(void)
{
    s32 v0;
    s32 a2;
    s32 v1;
    u8 *vp;
    s32 i;

    switch ((u32)step_mode) {
    case 0:
        card_step_init();
        step_mode = step_mode + 1;
        break;
    case 1:
        v0 = card_file_read_main();
        if (v0 == 3) {
            card_state_clear();
            step_mode = step_mode + 1;
            break;
        }
        if ((u32)card_cnt < 513) {
            if (card_result != 240) {
                return;
            }
        }
        card_state_clear();
        save_flag_clear();
        goto_sub_step(1);
        return;
    case 2:
        v0 = save_work_restore(0);
        if (v0 == -1) {
            step_mode = step_mode + 1;
            break;
        }
        text_sys_init();
        name_ent_card_io_noop();
        if (album_work[last_album + 4] == 1U) {
            a2 = last_album - 1;
        } else {
            a2 = 255;
            vp = &album_work[1];
            for (v1 = 1; v1 != 14; v1++) {
                if (vp[4] == 1U) {
                    last_album = v1;
                    a2 = v1 - 1;
                    break;
                }
                vp++;
            }
        }
        if (a2 != 255) {
            name_ent_cursor_set(a2);
        }
        name_ent_album_info_disp();
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        name_ent_view_page = 1;
        sub_sub_step_up();
        return;
    case 3:
        text_sys_init();
        set_kanji_string(-64, 32, 15, name_ent_load1_str0, 0);
        set_kanji_string(-64, 48, 15, name_ent_load1_str1, 0);
        k_disp_start(2);
        card_state_clear();
        step_mode = step_mode + 1;
        break;
    case 4:
        if (pad_trig & 0x60) {
            step_mode = step_mode + 1;
        }
        break;
    case 5:
        text_sys_init();
        set_kanji_string(-64, 32, 15, name_ent_load1_str2, 0);
        set_kanji_string(-64, 48, 15, name_ent_load1_str3, 0);
        set_kanji_string(-64, 64, 15, save_format_main_str6, 0);
        set_kanji_string(20, 64, 1, (u8 *)get_pad_str(), 0);
        set_kanji_string(-64, 80, 15, save_format_main_str7, 0);
        set_kanji_string(20, 80, 2, (u8 *)get_pad_str2(), 0);
        k_disp_start(6);
        step_mode = step_mode + 1;
        break;
    case 6:
        v0 = pad_trig;
        if (v0 & 0x20) {
            step_mode = step_mode + 1;
            break;
        }
        if ((v0 & 0x40) == 0) {
            break;
        }
        for (v1 = 0; v1 != 4; v1++) {
            if (card_file_delete(0) == 1) {
                break;
            }
        }
        goto_sub_step(1);
        return;
    case 7:
        goto_sub_step(208);
        return;
    default:
        break;
    }
}
