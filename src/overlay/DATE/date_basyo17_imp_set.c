/* @SymbolStart: 0x8013DC4C */
/* @SymbolSize: 144 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=3,4,2 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:t6 0x18:rt:v0:t6 0x20:rt:v0:t6 0x28:rt:v1:t7 0x2c:rt:v1:t7 0x34:rt:v1:t7 0x3c:rt:a0:t8 0x40:rt:a0:t8 0x48:rt:a0:t8 0x50:rt:v0:t6 0x5c:rt:v1:t7 0x6c:rt:a0:t8 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 kao2[];
extern s16 imp[];
extern s16 ans[];

void date_basyo17_imp_set(void) {
    (*(u16 *)&kao2[1]) = 3;
    (*(u16 *)&kao2[0]) = 3;
    (*(u16 *)&imp[1]) = 4;
    (*(u16 *)&imp[0]) = 4;
    (*(u16 *)&ans[1]) = 2;
    (*(u16 *)&ans[0]) = 2;
    (*(u16 *)&kao2[2]) = 3;
    (*(u16 *)&imp[2]) = 4;
    (*(u16 *)&ans[2]) = 2;
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
}
