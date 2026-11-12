/* Function: card_check_main */
/* Address: 0x80054308        (registered address; the TRUE entry after the */
/* Size: 496 bytes (124 instructions)  -> window is now 516 bytes / 129 insns */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x800542F4 */
/* @SymbolSize: 516 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_instructions --offset1 0x08 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x0c --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
/* @PostProc: patch_insn_reg_field 0x28:rt:v0:v1 0x2c:rs:v0:v1 0x2c:rt:v0:v1 0x34:rs:v0:v1 0x3c:rs:v0:v1 0x64:rs:v0:v1 0x6c:rt:v0:v1 0x14c:rt:v0:v1 0x150:rs:v0:v1 0x150:rt:v0:v1 0x158:rs:v0:v1 0x160:rs:v0:v1 0x168:rs:v0:v1 */
#include "types.h"

extern u8 card_busy;
extern s32 card_cnt;
extern u8 card_step;
extern u8 card_result;

extern void Sw_Clear(void);
extern s32 _card_info(s32 arg);
extern void card_step_next(void);
extern void card_event_step(s32 arg);
extern s32 _card_load(s32 arg);
extern s32 VSync(s32 arg);
extern void Hw_Clear(void);
extern s32 _card_clear(s32 arg);
extern void card_step_set(s32 arg);
extern void Hw_Test(void);

u8 card_check_main(void)
{
    u8 state;
    u8 val;

    card_cnt = card_cnt + 1;
    if (!card_busy) {
        goto done;
    }

    state = card_step;

    switch ((u32)state) {
    case 0:
        card_result = 0;
        Sw_Clear();
        if (_card_info(0) == 1) {
            card_step_next();
        }
        goto done;
    case 1:
        card_event_step(32);
        goto done;
    case 2:
        if (_card_load(0) == 1) {
            card_step_next();
        }
        goto done;
    case 3:
        card_event_step(64);
        goto done;
    case 4:
        card_busy = 0;
        card_result = 1;
        goto done;
    case 32:
        VSync(0);
        Hw_Clear();
        if (_card_clear(0) == 0) {
            card_step_set(0);
        } else {
            Hw_Test();
            val = card_result;
            switch (val) {
            case 0:
            case 129:
            default:
                card_step_set(0);
                break;
            case 136:
                card_step_set(48);
                goto done;
            }
        }
        goto done;
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
        goto done;
    case 48:
        if (_card_load(0) == 1) {
            card_step_next();
        }
        goto done;
    case 49:
        card_event_step(64);
        goto done;
    case 50:
        card_busy = 0;
        card_result = 1;
        goto done;
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
    case 63:
        goto done;
    case 64:
        card_busy = 0;
        card_result = 241;
        goto done;
    }

done:
    return card_result;
}
