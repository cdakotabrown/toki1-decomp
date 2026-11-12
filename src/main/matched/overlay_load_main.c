/* Function: overlay_load_main */
/* Address: 0x80078A9C  (registered address corrected 2026-09-17; see below) */
/* Size: 136 bytes (34 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2 */
/* The old file carried `@PostProc: swap_registers a0 v0`.  That paint existed */
/* @SymbolStart: 0x80078A9C */
/* @SymbolSize: 136 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count 4 */
#include "types.h"

extern u8 overlay_load_main_tbl0;

extern void overlay_load_init(void);
extern void overlay_load_wait(void);
extern void ending_load_gate(void);
extern void scene_entry_set(void);
extern void overlay_load_disp(void);

void overlay_load_main(void) {
    s32 mode = overlay_load_main_tbl0;

    switch (mode) {
    case 0:
        overlay_load_init();
        break;
    case 1:
        overlay_load_wait();
        break;
    case 2:
        ending_load_gate();
        break;
    case 3:
        scene_entry_set();
        break;
    }
    overlay_load_disp();
}
