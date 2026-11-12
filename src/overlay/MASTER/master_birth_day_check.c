/* @SymbolStart: 0x80135040 */
/* @SymbolSize: 104 */
/* master_birth_day_check - MASTER overlay, Address: 0x80135040, Size: 104 bytes (26 insns) */
/* @PostProc: patch_insn_reg_field 0x18:rt:t7:t6 0x1c:rs:t7:t6 0x1c:rt:t7:t6 0x24:rt:t6:t7 0x30:rt:t0:t8 0x34:rs:t0:t8 0x34:rt:t0:t8 0x38:rd:t8:t9 0x3c:rd:t9:t0 0x3c:rt:t8:t9 0x40:rs:t9:t8 */
#include "types.h"

extern void master_birth_day_main(void);
extern void sub_step_up(void);
extern void common_disp(void);

extern s32 girl0_birth;
extern u8 now_month;
extern u8 now_day;

void master_birth_day_check(void) {
    s32 packed;
    u8 code;
    u8 haircolor;
    u32 extract;

    packed = girl0_birth;
    code = now_month;
    if (code == (packed & 0xF)) {
        haircolor = now_day;
        extract = ((u32)(packed << 23)) >> 27;
        if (haircolor == extract) {
            master_birth_day_main();
            return;
        }
    }
    sub_step_up();
    common_disp();
}
