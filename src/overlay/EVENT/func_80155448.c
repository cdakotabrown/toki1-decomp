/* @SymbolStart: 0x80155440 */
/* @SymbolSize: 216 */
/* Address: 0x80155448 */
/* Size: 216 bytes (54 instructions, incl. the 2-insn hoisted head) */
/* @PostProc: patch_insn_reg_field 0x1c:rt:a0:v1 0x20:rs:a0:v1 0x28:rt:v1:v0 0x2c:rs:v1:v0 0x34:rs:a0:v0 0x34:rt:v0:v1 */
#include "types.h"

extern u8 D_800B1746;

extern s16 D_80094730;
extern volatile u8 D_800B0896;
extern u8 D_8012531C;

extern s16 D_8012438C;
extern s16 D_8012438E;
extern s16 D_80124390;
extern s16 D_8012439C;
extern s16 D_8012439E;
extern s16 D_801243A0;

extern void func_80011DFC(void);

void func_80155448(void) {
    u8 t6_v = D_800B1746;

    u8 branch;
    u8 charId;

    if (t6_v == 1) {
        branch = D_8012531C;
        if (branch == 1) {
            charId = D_800B0896;
            if ((u32)charId >= 6 && (u32)charId < 9) {
                D_80094730 += 9;
                D_8012439C = 1;
                D_8012439E = 3;
                D_801243A0 = 2;
                D_8012438C = 2;
                D_8012438E = 3;
                D_80124390 = 3;
            }
        } else if (branch == 2) {
            D_80094730 += 9;
        }
    }

    func_80011DFC();
}
