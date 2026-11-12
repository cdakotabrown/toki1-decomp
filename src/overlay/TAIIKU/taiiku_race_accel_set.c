/* taiiku_race_accel_set -- TAIIKU overlay, Address: 0x801449EC */
/* @SymbolStart: 0x801449E0 */
/* @SymbolSize: 160 */
/* @PostProc: patch_stack_complete --old 32 --new 40 */
/* @PostProc: remap_stack_slots --from 20 --to 24 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v0:v1 0x40:rs:v0:v1 0x40:rt:v0:v1 0x44:rd:v1:v0 0x58:rs:v0:v1 0x5c:rs:v1:v0 0x68:rs:v1:v0 0x6c:rt:v1:v0 0x9c:rs:v1:v0 0x9c:rt:v1:v0 0xa0:rs:v1:v0 0xa4:rs:v0:v1 0xa4:rt:v0:v1 */
#include "types.h"

typedef struct { u8 pad[36]; } E36;
typedef struct { s32 w[3]; } Src3;

extern Src3 taiiku_race_accel_set_tbl;
extern u8   class_w[];
extern E36  taiiku_ninin_run[];

void taiiku_race_accel_set(void)
{
    Src3 buf;
    u8 *r;
    u8 *p;
    s32 i;

    buf = taiiku_race_accel_set_tbl;
    r = class_w;
    for (i = 0; i != 3; i++) {
        if ((i + 54) != *(s16 *)(r + 6620)) {
            p = (u8 *)&taiiku_ninin_run[i];
            if (*(s32 *)(p + 64) != 0) {
                *(s32 *)(p + 44) = buf.w[i];
            } else {
                *(s32 *)(p + 44) = -1280;
            }
        }
        r += 68;
    }
}
