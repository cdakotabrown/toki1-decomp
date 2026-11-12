/* Address: 0x8013E0E4 */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern s32 D_800947C4;
extern s32 D_8011F830;

extern void func_80078970();
extern void func_80011DFC();

void func_8013E0E4(void) {
    func_80078970(&D_800947C4, &D_8011F830);
    func_80011DFC();
}
