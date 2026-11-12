/* @SymbolSize: 516 */
/* @PostProc: patch_stack_complete --old 216 --new 248 */
/* @PostProc: remap_stack_slots --map '104:108,132:140,160:172,188:204' */
/* @PostProc: patch_insn_reg_field 0x13c:rt:v1:s0 0x140:rt:a1:a0 0x144:rt:a0:v1 0x150:rs:a0:v1 0x158:rs:v1:s0 0x158:rt:v1:s0 0x15c:rs:v1:s0 0x15c:rt:a1:a0 0x1b0:rs:v1:s0 0x1b0:rt:a1:a0 0x1d0:rt:v1:s0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* Address: 0x80144A4C */
/* Size: 516 bytes (129 instructions) */
#include "types.h"

extern void goto_sub_sub_step(s32);
extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_sub_reset_point_set(void);
extern void menu_set(s32, s32, s16 *, s16 *, s16 *, s16 *);
extern void save_file_msg_disp(void);
extern void save_file_box_disp(void);
extern void basyo_init2(u8 *);
extern void album_info_disp(s32, s32);

extern u8 **card_file_save_tbl0;
extern s8 menu_csr;
extern u8 now_album;
extern u8 album_work[];
extern u8 album_status[];
extern u8 last_album;
extern u8 card_sys_present;
extern u8 save_file_slot_init_str0[];

void save_file_slot_init(void)
{
    s16 xs[14];
    s16 ys[14];
    s16 ws[14];
    s16 hs[14];
    s32 i;
    s32 k;
    s32 cur;

    text_sys_init();
    basyo_init2(save_file_slot_init_str0);

    for (i = 0; i < 14; i++) {
        xs[i] = (i / 5) * 32 - 124;
        ys[i] = (i % 5) * 16 - 64;
        ws[i] = 32;
        hs[i] = 16;
        set_kanji_string(xs[i], ys[i], 15, card_file_save_tbl0[i], 0);
    }

    menu_set(1, 14, xs, ys, ws, hs);
    menu_csr = 0;
    k_sub_reset_point_set();

    for (k = 1; k != 15; k++) {
        if (album_work[k + 4] == 1) {
            break;
        }
    }

    if (card_sys_present != 0) {
        cur = last_album;
        menu_csr = cur - 1;
        now_album = cur;
        goto_sub_sub_step(2);
        album_info_disp(0, 0);
        save_file_msg_disp();
    } else if (k == 15) {
        save_file_msg_disp();
        goto_sub_sub_step(192);
    } else {
        now_album = k;
        goto_sub_sub_step(192);
    }
    save_file_box_disp();
}
