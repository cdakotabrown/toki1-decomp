/* Address: 0x80059E20 */
/* Size: 40 bytes */
/* Region: GCC (0x80041000-0x80080FFF) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void main_data_vram_load(void);
extern void sub_sub_step_up(void);

void default_data_vram_load(void) {
    main_data_vram_load();
    sub_sub_step_up();
}
