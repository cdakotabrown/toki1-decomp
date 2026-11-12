/* @PostProc: patch_insn_reg_field 0x5c:rt:v0:t1 0x64:rt:v0:t1 0x68:rd:v1:t9 0x6c:rs:v1:t9 0x6c:rt:v1:t9 0x74:rs:v1:t9 */
/* ending_select_main - ENDING overlay, Address: 0x80133040, Size: 180 bytes (45 insns) */
#include "types.h"

typedef struct {
    s32 data[16];
} DispatchTable16;

extern DispatchTable16 ending_main0_tbl;

extern void bustup_ani();
extern void k_scroll_set();
extern void k_disp_wait();
extern void update_anim_state();
extern void message_window_show();
extern void basyo_show();
extern void hizuke_show();
extern void dir_scroll_main();

void ending_select_main(void)
{
    u8 t1;
    DispatchTable16 tbl;

    void (*fn)(s32);

    tbl = ending_main0_tbl;

    bustup_ani();

    t1 = *(u8 *)0x800EF86AU;
    fn = (void (*)(s32))tbl.data[t1];
    fn(128);

    k_scroll_set();
    k_disp_wait();
    update_anim_state(2);
    message_window_show();
    basyo_show();
    hizuke_show();
    dir_scroll_main();
}
