/* Address: 0x80046840 */
/* Size: 136 bytes (34 instructions) */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void system_reset(s32 arg0);
extern void goto_step(s32 arg0);
extern void prim_init(void);
extern void class_clear(s32 arg0);
extern void back_clear_switch(s32 arg0);
extern void dec_bg_reset(void);
extern void device_state_init(void);
extern void tpage_buf_clear(void);
extern void device_cmd_send(s32 arg0);
extern void SD_Exit(void);
extern void SD_Init(void);
extern void CDQ_init(void);
extern void _game_work_init_set(void);
extern void game_work_init2(void);

s32 game_reset(void)
{
    system_reset(0);
    goto_step(18);
    prim_init();
    class_clear(0);
    back_clear_switch(1);
    dec_bg_reset();
    device_state_init();
    tpage_buf_clear();
    device_cmd_send(127);
    SD_Exit();
    SD_Init();
    CDQ_init();
    _game_work_init_set();
    game_work_init2();
    return -1;
}
