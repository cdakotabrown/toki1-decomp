/* Address: 0x801330D4 */
/* Size: 40 bytes (10 insns) */
#include "types.h"

extern void taiiku_module_init2();
extern void taiiku_img_load();

void taiiku_race_vram_init(void)
{
    taiiku_module_init2();
    taiiku_img_load();
}
