/* @CompileFlags: -Wo,-loopunroll,0 */
/* Address: 0x80143574 */
/* Size: 208 bytes (52 insns) */
#include "types.h"

extern s32 srn_vram_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern s32 srn_init(s32 a0, s32 a1, s32 a2);
extern s32 mod_init(s32 a0, s32 a1, s32 a2);

extern s32 taiiku_ninin_buf;
extern s32 taiiku_ninin_buf_1;
extern s32 taiiku_ninin_buf_3;
extern s32 taiiku_ninin_buf_4;
extern s32 taiiku_ninin_buf_5;
extern s32 taiiku_ninin_buf_6;

extern s32 screen_w[];
extern u8 taiiku_ninin_girl1;

void taiiku_module_init4(void) {
    s32 t6 = taiiku_ninin_girl1;

    s32 i;

    if (t6 == 0) {
        mod_init(0, taiiku_ninin_buf_5, taiiku_ninin_buf);
        srn_init(0, taiiku_ninin_buf_3, 0);
    } else {
        mod_init(0, taiiku_ninin_buf_6, taiiku_ninin_buf_1);
        srn_init(0, taiiku_ninin_buf_4, 0);
    }

    srn_vram_set(0, 5, 1408, 240);

    for (i = 0; i < 3; i++) {
        *(s32 *)((s8 *)&screen_w[i] + 4668) = 8;
    }
    for (i = 0; i < 3; i++) {
        *(s32 *)((s8 *)&screen_w[i] + 4796) = 8;
    }
}
