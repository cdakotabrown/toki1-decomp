/* @PostProc: patch_stack_complete --old 56 --new 72 */
/* @PostProc: remap_stack_slots --map '32:52,36:56,40:60,44:64' */
/* @PostProc: patch_insn_reg_field 0x50:rt:v0:t1 0x58:rs:v0:t1 0xb0:rt:t2:at 0xb8:rs:t2:at */
/* ending_song_init - ENDING overlay, Address: 0x8013BBCC, Size: 188 bytes (47 instructions) */
#include "types.h"

typedef struct {
    s32 a0, a1, a2, a3;
} FourWords;

extern FourWords ending_song_init_tbl;
extern u8 ending_song_init_str0[];

extern void device_cmd_send();
extern void set_XAsector();
extern s32 set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

void ending_song_init(void)
{
    s32  result;
    u8   flag;
    FourWords tbl;

    tbl = ending_song_init_tbl;

    device_cmd_send(191);

    flag = *(u8 *)0x800EED1EU;
    if (flag != 0) {
        set_XAsector(tbl.a2, tbl.a3, 15);
    } else {
        set_XAsector(tbl.a0, tbl.a1, 15);
    }

    device_cmd_send(768);
    result = set_kanji_string(-80, 80, 0, (s32)ending_song_init_str0, 0);
    k_disp_start(result);
    *(s16 *)0x8013CC10U = 0;
    sub_sub_step_up();
}
