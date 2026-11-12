/* Address: 0x80142DBC */
/* Size: 40 bytes (10 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void main_data_vram_load();

void taco_main_data_vram_load_step(void)
{
    main_data_vram_load();
    sub_sub_step_up();
}
