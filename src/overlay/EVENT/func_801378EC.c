/* Address: 0x801378EC */
/* Size: 52 bytes (13 instructions) */
#include "types.h"

extern s32 D_800947C4;
extern s32 D_8011F600;

extern void func_80078970();
extern void func_80011DFC();

void func_801378EC(void) {
    func_80078970(&D_800947C4, &D_8011F600);
    func_80011DFC();
}
