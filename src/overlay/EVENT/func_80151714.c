/* @CompileEnv: IDO52_CONST_INREG_VALUES=1,2,3 IDO52_FORM_SITE=const:2:28:0 */
/* @PostProc: swap_instructions --offset1 0x94 --offset2 0x98 */
/* @PostProc: patch_insn_reg_field 0x3c:rt:v0:t6 0x44:rt:v0:t6 0x4c:rt:v0:t6 0x64:rt:v0:t6 0x6c:rt:t6:t8 0x70:rs:t6:t8 0x70:rt:t6:t8 0x7c:rs:t6:t8 0x80:rt:v0:t9 0x88:rt:v0:t9 0x90:rt:v0:t9 0x98:rt:v1:t0 0x9c:rt:v1:t0 0xa4:rt:v1:t0 0xac:rt:v0:t9 0xb4:rt:v1:t0 */
/* Address: 0x80151714 */
/* Size: 192 bytes (48 instructions) */
/* scans the whole file, so an '@PostProc:' inside prose is applied twice) */
#include "types.h"

extern s16 D_80124394;
extern s16 D_80124396;
extern s16 D_80124398;
extern s16 D_801243A4;
extern s16 D_801243A6;
extern s16 D_801243A8;
extern u8 D_800B1746;
extern s32 D_800947C4;
extern s32 D_8011FED4;

extern void func_80078970(s32 *, s32 *);
extern void func_800433D0(s32);
extern void func_800469F4(s32);
extern void func_80011DFC(void);

void func_80151714(void) {
    func_80078970(&D_800947C4, &D_8011FED4);
    func_800433D0(516);
    func_800469F4(17115);
    D_80124396 = 1;
    D_80124394 = 1;
    D_801243A6 = 0;
    D_801243A4 = 0;
    D_80124398 = 1;
    D_801243A8 = 0;
    if (D_800B1746 == 2) {
        D_80124396 = 3;
        D_80124394 = 3;
        D_801243A6 = 2;
        D_801243A4 = 2;
        D_80124398 = 3;
        D_801243A8 = 2;
    }
    func_80011DFC();
}
