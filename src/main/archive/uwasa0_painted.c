/* Function: func_80060E28 */
/* Address: 0x80060E28 */
/* Size: 76 bytes (19 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: patch_stack_complete --old 24 --new 40 */
/* @PostProc: swap_registers a0 v1 */
/* @PostProc: swap_instructions 0x4 0x8 */
/* @PostProc: custom_script custom_func_80060E28.py */
#include "types.h"

extern s32 uwasa_main0(void);
extern s32 uwasa_exit0(void);

s32 func_80060E28(s32 mode) {
    switch (mode) {
    case 0:
        return uwasa_main0();
    case 1:
        return uwasa_exit0();
    }
    return mode;
}
