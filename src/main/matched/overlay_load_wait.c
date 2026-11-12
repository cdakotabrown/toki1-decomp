/* Address: 0x8007906C */
/* Size: 52 bytes */
/* Region: GCC (OR encoding, < 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_CD_State2(void);
extern u8 overlay_load_main_tbl0;

void overlay_load_wait(void) {
    if (get_CD_State2() & 1) {
        overlay_load_main_tbl0 = 2;
    }
}
