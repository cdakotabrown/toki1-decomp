/* Address: 0x80135824 */
/* Size: 152 bytes (38 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"

extern s32 taiiku_kari_buf;
extern s32 taiiku_kari_buf_2;
extern s32 taiiku_kari_buf_3;

extern s32 screen_w[3];

extern s32 srn_vram_set(s32, s32, s32, s32);
extern s32 srn_init(s32, s32, s32);
extern s32 mod_init(s32, s32, s32);

void taiiku_module_init3(void) {
    s32 i;

    mod_init(0, taiiku_kari_buf_3, taiiku_kari_buf);
    srn_init(0, taiiku_kari_buf_2, 0);
    srn_vram_set(0, 5, 1408, 240);

    for (i = 0; i < 3; i++) {
        *(s32 *)((s8 *)&screen_w[i] + 4668) = 8;
    }
    for (i = 0; i < 3; i++) {
        *(s32 *)((s8 *)&screen_w[i] + 4796) = 8;
    }
}
