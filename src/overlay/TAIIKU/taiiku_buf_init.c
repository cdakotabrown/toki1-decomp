/* Address: 0x80145350 */
/* Size: 116 bytes (29 insns) */
/* @SymbolSize: 116 */
#include "types.h"

extern s32 taiiku_ninin_buf;
extern s32 taiiku_ninin_buf_1;
extern s32 taiiku_ninin_buf_2;
extern s32 taiiku_ninin_buf_3;
extern s32 taiiku_ninin_buf_4;
extern s32 taiiku_ninin_buf_5;
extern s32 taiiku_ninin_buf_6;

void taiiku_buf_init(void) {
    taiiku_ninin_buf = 0x801AD000;
    taiiku_ninin_buf_1 = 0x801AE018;
    taiiku_ninin_buf_2 = 0x8018F000;
    taiiku_ninin_buf_3 = 0x801AF1B0;
    taiiku_ninin_buf_4 = 0x801B29EC;
    taiiku_ninin_buf_5 = 0x801AD008;
    taiiku_ninin_buf_6 = 0x801AE020;
}
