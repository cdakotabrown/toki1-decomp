/* Address: 0x8013A024 */
/* Size: 88 bytes (22 insns) */
#include "types.h"

extern s32 taiiku_tama_buf;
extern s32 taiiku_tama_buf_2;
extern s32 taiiku_tama_buf_3;

extern s32 mod_init();
extern s32 srn_init();
extern s32 srn_vram_set();

void taiiku_vram_init(void)
{
    mod_init(0, taiiku_tama_buf_3, taiiku_tama_buf);
    srn_init(0, taiiku_tama_buf_2, 0);
    srn_vram_set(0, 5, 1408, 240);
}
