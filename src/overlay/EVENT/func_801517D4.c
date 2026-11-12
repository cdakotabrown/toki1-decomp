/* Address: 0x801517D4 */
/* Size: 68 bytes (17 instructions) */
#include "types.h"

extern s32 D_800947C4;
extern s32 D_8011FEE4;

extern void func_800469F4();
extern void func_80078970();
extern void func_80011DFC();

void func_801517D4(void) {
    func_800469F4(17051);
    func_80078970(&D_800947C4, &D_8011FEE4);
    func_80011DFC();
}
