/* Address: 0x800790A0 */
/* Size: 92 bytes (23 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80041000-0x80080FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern u8 overlay_load_main_tbl0;
extern u8 ending_main_tbl0;
extern s32 ending_main_tbl1;
extern s32 overlay_load_init_tbl0;

extern s32 block_checksum_check(u8, s32);
extern void cache_flush_critical(void);

void ending_load_gate(void) {
    s32 result;

    result = block_checksum_check(ending_main_tbl0, ending_main_tbl1);
    if (result != 0) {
        overlay_load_main_tbl0 = 3;
        cache_flush_critical();
    } else {
        overlay_load_main_tbl0 = 0;
        overlay_load_init_tbl0 = 0;
    }
}
