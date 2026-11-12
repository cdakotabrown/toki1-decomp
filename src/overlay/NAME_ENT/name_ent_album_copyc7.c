/* Address: 0x8013F8C8 (absorbed cross-prologue head) .. 0x8013FAF4 */
/* Size: 556 bytes (139 instructions) */
/* @SymbolStart: 0x8013F8C8 */
/* @SymbolSize: 556 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: swap_instructions --offset1 0xfc --offset2 0x100 */
/* @PostProc: patch_insn_reg_field 0xc8:rt:t8:v1 0xcc:rs:t8:v1 0xcc:rt:t8:v1 0xd4:rs:t8:v1 0xd4:rt:t9:t8 0xdc:rt:t9:t8 0xf0:rt:t4:t2 0xf4:rt:t1:t0 0xf8:rs:t1:t0 0xf8:rt:t1:t0 0xfc:rt:t2:v1 0x104:rs:t2:v1 0x104:rt:t2:v1 0x108:rt:t0:t9 0x10c:rt:t1:t0 0x110:rt:t0:t9 0x118:rs:t2:v1 0x118:rt:t3:t1 0x120:rt:t3:t1 0x12c:rs:t4:t2 0x12c:rt:t4:t2 0x130:rt:t5:t3 0x134:rs:t4:t2 0x140:rs:t5:t3 0x140:rt:t5:t3 0x148:rs:t5:t3 0x180:rt:t6:t4 0x184:rs:t6:t4 0x184:rt:t6:t4 0x18c:rt:t6:t4 0x1a4:rt:t7:v1 0x1a8:rs:t7:v1 0x1a8:rt:t7:v1 0x1b0:rs:t7:v1 0x1b0:rt:t8:t5 0x1b8:rt:t8:t5 0x1cc:rt:t0:t6 0x1e0:rt:t9:v1 0x1ec:rt:t9:v1 0x1f0:rs:t0:t6 0x1f0:rt:t0:t6 0x1f4:rt:t1:t7 0x1f8:rs:t0:t6 0x204:rs:t1:t7 0x204:rt:t1:t7 0x20c:rs:t1:t7 */
#include "types.h"

extern s32  text_sys_init(void);
extern void strcpy(s32 *buf, s32 a1);
extern void strcat(s32 *buf, u8 *fmt);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, void *a3, s32 a4);
extern void k_disp_start(s32 a0);
extern s32  card_step_init(void);
extern s32  card_slot_write_main(void);
extern s32  card_file_write_main(void);
extern s32  card_state_clear(void);
extern void save_flag_clear(void);
extern s32  goto_sub_step(s32 a0);
extern s32  goto_sub_sub_step(s32 a0);
extern s32  save_state_set(void);
extern void printf(u8 *a0);

extern u8  now_album;
extern u8  step_mode;
extern s32 album_info_disp_tbl0[];
extern u8  name_ent_album_copyc7_str0;
extern u8  name_ent_album_copyc7_str1;
extern u8  name_ent_album_copyc7_str2;
extern s32 card_cnt;
extern u8  card_result;
extern u8  last_album;
extern u8  album_status[];

s32 name_ent_album_copyc7(void)
{
    s32 r;
    s32 sel;
    s32 buf[64];

    sel = step_mode;
    r = sel;
    switch (sel) {
    case 0:
        text_sys_init();
        strcpy(buf, album_info_disp_tbl0[now_album]);
        strcat(buf, &name_ent_album_copyc7_str0);
        set_kanji_string(-64, 32, 15, buf, 0);
        set_kanji_string(-64, 48, 15, &name_ent_album_copyc7_str1, 0);
        k_disp_start(2);
        r = card_step_init();
        step_mode++;
        break;
    case 1:
        r = card_slot_write_main();
        if (r == 20) {
            album_status[now_album] = 1;
            step_mode++;
            r = card_state_clear();
        } else if ((u32)card_cnt >= 513 || card_result == 240) {
            printf(&name_ent_album_copyc7_str2);
            card_state_clear();
            save_flag_clear();
            r = goto_sub_step(1);
        }
        break;
    case 2:
        card_state_clear();
        last_album = now_album;
        now_album = 0;
        card_step_init();
        r = save_state_set();
        step_mode++;
        break;
    case 3:
        r = card_file_write_main();
        if (r == 19) {
            card_state_clear();
            r = goto_sub_sub_step(0);
            step_mode = 2;
        } else if ((u32)card_cnt >= 513 || card_result == 240) {
            card_state_clear();
            save_flag_clear();
            r = goto_sub_step(1);
        }
        break;
    }
    return r;
}
