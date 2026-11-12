/* @SymbolStart: 0x80132000 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* @SymbolSize: 96 */
/* tel_main - TEL overlay, Address: 0x80132008, Size: 88 bytes (22 insns) */
#include "types.h"

extern u8 now_sub_step;

extern void tel_yoshio_main(void);
extern void tel_dialog_dispatch(void);
extern void game_reset(void);

void tel_main(void) {
    u8 hidden_v1;

    hidden_v1 = now_sub_step;
    switch (hidden_v1) {
        case 0:
            tel_yoshio_main();
            break;
        case 1:
            tel_dialog_dispatch();
            break;
        default:
            game_reset();
            break;
    }
}
