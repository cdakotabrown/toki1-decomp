/* @SymbolSize: 224 */
/* Function: date_basyo_load_wait */
/* Overlay: DATE */
/* Address: 0x8013EDC8 */
/* Size: 224 bytes (56 instructions) */
#include "types.h"

extern s32 device_load_poll(void);
extern void device_cmd_send(s32);
extern void sub_sub_step_up(void);
extern void CDQ_init(void);
extern void device_state_init(void);
extern void goto_sub_sub_step(s32);

typedef struct {
    u16 e[21];
} Table;

extern Table date_basyo_load_wait_tbl;
extern u8 now_basyo;
extern s32 step_counter;
extern u8 now_sub_sub_step;

void date_basyo_load_wait(void) {
    s32 v1;
    Table local_table;
    s32 v0;
    u8 a0_val;

    local_table = date_basyo_load_wait_tbl;

    v0 = device_load_poll();
    if (v0 == 1) {
        device_cmd_send(local_table.e[now_basyo]);
        sub_sub_step_up();
    }

    v1 = step_counter;
    v0 = (u32)v1 >= 1024u;
    v1++;
    step_counter = v1;

    if (v0 != 0) {
        CDQ_init();
        device_state_init();
        a0_val = now_sub_sub_step - 1;
        goto_sub_sub_step((u8)a0_val);
    }
}
