/* @SymbolStart: 0x8013D1E0 */
/* @RodataBase: 0x8014C194 */
/* @PostProc: patch_insn_reg_field 0x74:rt:t1:a1 0x98:rs:t1:a1 0xa4:rt:t2:t1 0xc0:rs:t2:t1 0xc0:rt:t2:t1 0xc8:rs:t2:t1 0x104:rt:t5:t4 0x108:rt:t3:t2 0x10c:rs:t3:t2 0x10c:rt:t3:t2 0x114:rs:t3:t2 0x114:rt:t4:t3 0x11c:rt:t4:t3 0x120:rt:t6:t5 0x124:rs:t6:t5 0x124:rt:t6:t5 0x12c:rt:t6:t5 0x130:rt:t5:t4 0x154:rt:t0:a1 0x184:rt:t7:t6 0x188:rs:t7:t6 0x188:rt:t7:t6 0x194:rt:t7:t6 0x198:rt:t8:t7 0x19c:rs:t8:t7 0x19c:rt:t8:t7 0x1a4:rs:t8:t7 0x1a4:rt:t9:t8 0x1ac:rt:t9:t8 0x1b0:rs:t0:a1 0x1c0:rt:t2:t0 0x1c4:rs:t2:t0 0x1c4:rt:t2:t0 0x1cc:rt:t1:t9 0x1d0:rt:t2:t0 0x1d4:rt:t1:t9 0x1f8:rt:t3:t1 0x214:rs:t3:t1 0x214:rt:t3:t1 0x218:rt:t4:a1 0x21c:rs:t3:t1 0x228:rs:t4:a1 0x228:rt:t4:a1 0x230:rs:t4:a1 */
/* @SymbolSize: 600 */
#include "types.h"

extern u8 step_mode;
extern u8 card_result;
extern u8 now_album;
extern u8 last_album;
extern s32 card_cnt;
extern u8 album_work[];

extern u8 name_ent_save3_str0[];
extern u8 name_ent_memstat_fmt[];

extern void card_step_init(void);
extern s32 card_file_read_main2(void);
extern void card_state_clear(void);
extern void save_flag_clear(void);
extern void goto_sub_step(s32 arg);
extern s32 save_check(s32 arg);
extern s32 save_game_restore(s32 arg);
extern void name_ent_card_io_noop(void);
extern void printf();
extern void device_cmd_send(s32 arg);
extern void save_state_set(void);
extern s32 card_file_write_main(void);
extern void name_ent_status_disp(void);

void name_ent_save3(void)
{
    s32 v0;
    s32 s1;
    u8 *base;

    switch ((u32)step_mode) {
    case 0:
        card_step_init();
        step_mode = step_mode + 1;
        break;
    case 1:
        v0 = card_file_read_main2();
        if (v0 == 4) {
            card_state_clear();
            step_mode = step_mode + 1;
            break;
        }
        s1 = *(u8 *)((u8 *)0x800F0000 - 1931);
        if (s1 == 240) {
            save_flag_clear();
            goto_sub_step(1);
            break;
        }
        if ((u32)card_cnt < 513) {
            if (s1 != 208) {
                break;
            }
        }
        card_state_clear();
        goto_sub_step(255);
        break;
    case 2:
        v0 = save_check(0);
        if (v0 == 1) {
            step_mode = step_mode + 1;
            break;
        }
        album_work[now_album + 4] = 255;
        goto_sub_step(240);
        break;
    case 3:
        v0 = save_game_restore(0);
        base = (u8 *)0x800F0000;
        if (v0 == 1) {
            name_ent_card_io_noop();
            printf(name_ent_save3_str0);
            card_state_clear();
            device_cmd_send(1281);
            card_step_init();
            last_album = now_album;
            save_state_set();
            step_mode = step_mode + 1;
            break;
        }
        s1 = base[-1931];
        printf(name_ent_memstat_fmt, s1);
        album_work[now_album + 4] = 255;
        goto_sub_step(240);
        break;
    case 4:
        v0 = card_file_write_main();
        if (v0 == 19) {
            card_state_clear();
            goto_sub_step(3);
            break;
        }
        if ((u32)card_cnt < 513) {
            if (card_result != 240) {
                break;
            }
        }
        card_state_clear();
        save_flag_clear();
        goto_sub_step(1);
        break;
    default:
        break;
    }
    name_ent_status_disp();
}
