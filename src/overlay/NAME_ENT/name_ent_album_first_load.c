/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x8013D438 */
/* @SymbolSize: 544 */
/* @PostProc: patch_stack_complete --old 328 --new 336 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x24:rt:at:v0 0x28:rs:v0:v1 0x28:rt:at:v0 0x30:rs:v0:v1 0x48:rt:v1:a0 0x4c:rs:v1:a0 0x4c:rt:v1:a0 0x50:rt:v0:v1 0x58:rt:a0:v0 0x5c:rs:v1:a0 0x64:rs:a0:v0 0x70:rt:v0:v1 0x74:rs:v0:v1 0x74:rt:v0:v1 0x78:rs:v0:v1 0x7c:rs:v1:a0 0x7c:rt:v1:a0 0x88:rs:v1:a0 0x90:rt:v0:v1 0x94:rs:v0:v1 0x94:rt:v0:v1 0x9c:rs:v0:v1 0xe4:rt:v0:v1 0xe8:rs:v0:v1 0xe8:rt:v0:v1 0xf0:rs:v0:v1 0x16c:rt:a0:at 0x170:rt:a0:at */
/* Address: 0x8013D438 (absorbed head) / 0x8013D440 (prologue) */
/* Size: 536 bytes (134 insns), 544 bytes / 136 words including the head */
#include "types.h"

extern void card_step_init(void);
extern void card_state_clear(void);
extern void goto_sub_sub_step(s32 arg);
extern void printf(u8 *str, s32 value);
extern s32  card_file_read_main2(void);
extern void save_flag_clear(void);
extern void goto_sub_step(s32 arg);
extern s32  save_check(s32 arg);
extern void name_ent_album_disp_init(void);
extern void sub_sub_step_up(void);
extern void text_sys_init(void);
extern void strcpy(void *buf, s32 value);
extern void strcat(void *buf, u8 *str);
extern void set_kanji_string(s32 x, s32 y, s32 w, void *src, s32 flag);

extern u8   step_mode;
extern u8   now_album;
extern u8   card_result;
extern s32  card_cnt;
extern u8   album_status[15];
extern s32  album_info_disp_tbl0[];
extern u8   name_ent_load_file_fmt[];
extern u8   name_ent_alb_data_str[];
extern u8   name_ent_alb_broken_str[];

void name_ent_album_first_load(void)
{
    u8 buf[256];
    u32 step;
    s32 i;
    u8 *p;
    s32 code;

    step = step_mode;

    switch (step) {
    case 0:
        card_step_init();
        i = 1;
        p = album_status - 3;
        do {
            if (p[4] == 1) {
                now_album = i;
                break;
            }
            i++;
            p++;
        } while (i != 15);
        if (p < album_status + 11) {
            step_mode++;
        } else {
            card_state_clear();
            goto_sub_sub_step(64);
        }
        printf(name_ent_load_file_fmt, now_album);
        break;

    case 1:
        if (card_file_read_main2() == 4) {
            step_mode++;
            break;
        }
        code = card_result;
        if (code == 240) {
            save_flag_clear();
            goto_sub_step(1);
            break;
        }
        if ((u32)card_cnt < 513) {
            if (code != 208) {
                break;
            }
        }
        card_state_clear();
        save_flag_clear();
        goto_sub_step(1);
        break;

    case 2:
        if (save_check(0) == 1) {
            name_ent_album_disp_init();
            sub_sub_step_up();
            break;
        }
        text_sys_init();
        strcpy(buf, album_info_disp_tbl0[now_album]);
        strcat(buf, name_ent_alb_data_str);
        set_kanji_string(-64, 32, 15, buf, 0);
        set_kanji_string(-64, 48, 15, name_ent_alb_broken_str, 0);
        album_status[now_album] = 255;
        card_state_clear();
        step_mode = 0;
        break;
    }
}
