/* Address: 0x80134460 */
/* Size: 176 bytes (44 insns) */
/* @SymbolSize: 176 */
#include "types.h"

extern void load_image_rect();
extern void taco_tile_load();
extern void DrawSync();

void tile_pages_load(void)
{
    void *ptr = (void *)0x801C8000;

    load_image_rect(256, 480, 256, 24, (void *)0x801B8000);
    load_image_rect(0, 496, 256, 16, (void *)0x801C0000);
    taco_tile_load(ptr, 8);
    taco_tile_load(ptr, 10);
    taco_tile_load(ptr, 12);
    taco_tile_load(ptr, 14);
    taco_tile_load(ptr, 24);
    taco_tile_load(ptr, 26);
    DrawSync(0);
}
