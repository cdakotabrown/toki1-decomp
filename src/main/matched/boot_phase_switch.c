/* Function: boot_phase_switch */
/* Address: 0x80059DA8 */
/* Size: 120 bytes (30 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Compiler: IDO 5.2 */
/* @SymbolStart: 0x80059DA0 */
/* @SymbolSize: 128 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v1:4 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void boot_data_load(void);
extern void default_data_vram_load(void);
extern void debug_font_load(void);
extern void sub_sub_step_up(void);
extern void sub_step_up(void);

void boot_phase_switch(void)
{
    s32 mode;

    mode = now_sub_sub_step;
    switch (mode) {
    case 0:
        boot_data_load();
        break;
    case 1:
        default_data_vram_load();
        break;
    case 2:
        debug_font_load();
        sub_sub_step_up();
        break;
    default:
        sub_step_up();
        break;
    }
}
