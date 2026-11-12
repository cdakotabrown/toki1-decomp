/* Address: 0x80054994        (registered address; the TRUE entry after the */
/* Size: 240 bytes (60 instructions)  -> window is now 260 bytes / 65 insns */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x80054980 */
/* @SymbolSize: 260 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: swap_instructions --offset1 0x08 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x0c --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers t8 t0 --exclude-offsets 0x0,0x4,0x8,0xc,0x10,0x14,0x18,0x1c,0x20 */
/* @PostProc: swap_registers t6 t8 --exclude-offsets 0x0,0x4,0x8,0xc,0x10,0x14,0x18,0x1c,0x20 */
/* @PostProc: swap_registers t9 t1 --exclude-offsets 0x0,0x4,0x8,0xc,0x10,0x14,0x18,0x1c,0x20 */
/* @PostProc: swap_registers t7 t9 --exclude-offsets 0x0,0x4,0x8,0xc,0x10,0x14,0x18,0x1c,0x20 */
/* @PostProc: patch_insn_reg_field 0x80:rt:t1:t9 0x98:rt:t1:t9 */
#include "types.h"

extern u8 card_busy;
extern s32 card_cnt;
extern u8 card_step;
extern u8 now_album;
extern u8 card_result;

extern void Sw_Clear(void);
extern s32 card_file_write(u8);
extern void card_step_next(void);
extern s32 card_status_wait(void);

u8 card_slot_write_main(void) {
    card_cnt = card_cnt + 1;
    if (!card_busy) {
        goto done;
    }

    switch (card_step) {
    case 0:
        Sw_Clear();
        if (card_file_write(now_album) == 1) {
            card_step_next();
            break;
        }
        card_result = 208;
        card_busy = 0;
        goto done;
    case 1:
        if (card_status_wait() == 1) {
            card_step_next();
        }
        break;
    case 2:
        card_result = 20;
        card_busy = 0;
        goto done;
    case 16:
        card_busy = 0;
        card_result = 240;
        goto done;
    }

done:
    return card_result;
}
