/* Address: 0x8013FAF4 (absorbed cross-prologue head) .. 0x8013FC78 */
/* Size: 388 bytes (97 instructions) */
/* @SymbolStart: 0x8013FAF4 */
/* @SymbolSize: 388 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: swap_instructions --offset1 0xf4 --offset2 0xf8 */
/* @PostProc: patch_insn_reg_field 0xc0:rt:t8:v1 0xc4:rs:t8:v1 0xc4:rt:t8:v1 0xcc:rs:t8:v1 0xcc:rt:t9:t8 0xd4:rt:t9:t8 0xe8:rt:t4:t2 0xec:rt:t1:t0 0xf0:rs:t1:t0 0xf0:rt:t1:t0 0xf4:rt:t2:v1 0xfc:rs:t2:v1 0xfc:rt:t2:v1 0x100:rt:t0:t9 0x104:rt:t1:t0 0x108:rt:t0:t9 0x110:rs:t2:v1 0x110:rt:t3:t1 0x118:rt:t3:t1 0x124:rs:t4:t2 0x124:rt:t4:t2 0x128:rt:t5:t3 0x12c:rs:t4:t2 0x138:rs:t5:t3 0x138:rt:t5:t3 0x140:rs:t5:t3 0x178:rt:t6:v1 0x180:rt:t6:v1 */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern u8 now_album;
extern u32 card_cnt;
extern u8 album_status[];
extern u32 album_info_disp_tbl0[];
extern u8 name_ent_album_copyc8_str0[];
extern u8 name_ent_album_copyc8_str1[];
extern u8 name_ent_album_copyc8_str2[];

extern void text_sys_init(void);
extern void strcpy(u8 *, u32);
extern void strcat(u8 *, u8 *);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern void k_disp_start(s32);
extern s32  card_step_init(void);
extern s32  card_slot_write_main(void);
extern s32  card_state_clear(void);
extern void printf(u8 *);
extern void save_flag_clear(void);
extern s32  goto_sub_step(s32);
extern s32  goto_sub_sub_step(s32);

s32 name_ent_album_copyc8(void)
{
    s32 r;
    s32 sel;
    u8 buf[256];

    sel = step_mode;
    r = sel;
    switch (sel) {
    case 0:
        text_sys_init();
        strcpy(buf, album_info_disp_tbl0[now_album]);
        strcat(buf, name_ent_album_copyc8_str0);
        set_kanji_string(-64, 32, 15, buf, 0);
        set_kanji_string(-64, 48, 15, name_ent_album_copyc8_str1, 0);
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
            printf(name_ent_album_copyc8_str2);
            card_state_clear();
            save_flag_clear();
            r = goto_sub_step(1);
        }
        break;
    case 2:
        r = goto_sub_sub_step(0);
        step_mode = 2;
        break;
    }
    return r;
}
