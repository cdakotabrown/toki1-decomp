/* Address: 0x80152EBC */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern s32 D_800947C4;
extern s32 D_8011FF7C;

extern void func_80078970();
extern void func_80011DFC();

void func_80152EBC(void) {
    func_80078970(&D_800947C4, &D_8011FF7C);
    func_80011DFC();
}
