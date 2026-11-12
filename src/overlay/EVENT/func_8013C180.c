/* Address: 0x8013C180 */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s32 D_80094764;
extern s32 D_8011F78C;
extern s16 D_800EAFB6;

extern void func_80078970();
extern void func_80011DFC();

void func_8013C180(void) {
    func_80078970(&D_80094764, &D_8011F78C);
    D_800EAFB6 = 5;
    func_80011DFC();
}
