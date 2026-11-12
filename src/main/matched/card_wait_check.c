/* Address: 0x8006CE74 */
/* Size: 172 bytes (43 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers_liveness --address 0x8006CE74 */
#include "types.h"

extern u8 card_mode;
extern u32 card_cnt;
extern u8 card_result;
extern s32 card_file_write_main(void);
extern void device_cmd_send(s32);
extern void card_state_clear(void);

s32 card_wait_check(void) {
    s32 mode;

    mode = card_mode;
    if (mode == 1) {
        if (card_file_write_main() == 19) {
            device_cmd_send(1295);
            card_mode = 0;
            card_state_clear();
        } else if (card_cnt >= 513 || card_result == 240) {
            device_cmd_send(1282);
            card_mode = 0;
            card_state_clear();
        }
    } else if (mode == 0) {
        return 1;
    }
    return 0;
}
