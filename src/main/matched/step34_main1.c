/* Address: 0x8005B0F0 */
/* Size: 200 bytes (50 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v0 v1 --exclude-offsets 0x44 */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 card_mode;
extern u8 card_result;
extern u32 card_cnt;
extern void step34_disp(void);
extern void sprite_state_set(void);
extern s32 card_file_write_main(void);
extern void device_cmd_send(s32);
extern void card_state_clear(void);
extern void icon_mem_card_switch(s32);

void step34_main1(void) {
    if (now_sub_sub_step == 0) {
        step34_disp();
    }
    sprite_state_set();
    if (card_mode == 1) {
        if (card_file_write_main() == 19) {
            device_cmd_send(1295);
            card_mode = 0;
            card_state_clear();
        } else {
            if (card_cnt >= 513 || card_result == 240) {
                device_cmd_send(1282);
                card_mode = 0;
                card_state_clear();
            }
        }
    } else if (card_mode == 0) {
        icon_mem_card_switch(0);
    }
}
