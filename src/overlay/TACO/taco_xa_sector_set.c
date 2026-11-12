/* `@CompileFlags: -Wc,-pic0` with the boilerplate "this TU accounts for */
/* @CompileEnv: IDO52_AS1_FORCE_NARROW_FOLD=0 */
/* @PostProc: patch_insn_reg_field 0x10:rd:a3:t6 0x14:rt:t7:t8 0x18:rs:t7:t8 0x18:rt:t7:t8 0x1c:rd:t6:t7 0x1c:rt:a3:t6 0x20:rs:t6:t7 0x20:rt:t7:t8 */
/* Address: 0x80137CCC (absorbed head)  Size: 68 bytes (17 instructions) */
/* @SymbolStart: 0x80137CCC */
/* @SymbolSize: 68 */
#include "types.h"

typedef struct {
    s32 field_0;
    s32 field_4;
} Entry80137CE0;

extern Entry80137CE0 taco_xa_sector_tbl[];

extern void set_XAsector();
extern void device_cmd_send();

void taco_xa_sector_set(u8 idx) {
    Entry80137CE0 *e;

    e = &taco_xa_sector_tbl[idx];
    set_XAsector(e->field_0, e->field_4, 8);
    device_cmd_send(768);
}
