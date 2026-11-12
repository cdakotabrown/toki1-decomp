/* @SymbolStart: 0x8013876C */
/* @SymbolSize: 384 */
/* @CompileFlags: -Wo,-loopunroll,0 */
#include "types.h"
#include "tokimeki/class_w.h"

typedef struct { s32 w[4]; } Rec16;
typedef struct { s32 w[11]; } Buf44;
typedef struct { s16 f; u8 pad[66]; } Rec68;

extern Rec16 get_taiiku_rank_frame_tbl0;
extern Buf44 get_taiiku_rank_frame_tbl1;
extern u8 taiiku_kari_rank[];
extern s32 taiiku_konami_mode;
extern u8 taiiku_kari_girl;
extern s16 taiiku_kari_gl_card[];
extern u8 taiiku_kari_run_0_seat[][36];
extern CLASS_W class_w[];

s32 get_taiiku_rank_frame(s32 a0, s32 a1) {
    s32 v0;
    s32 v1;
    Rec16 buf16;
    Buf44 buf44;
    s32 p16;

    p16 = (s32)&buf16;
    buf16 = get_taiiku_rank_frame_tbl0;
    buf44 = get_taiiku_rank_frame_tbl1;

    v0 = a0 + 1;
    if (v0 == taiiku_kari_rank[0]) {
        v1 = 0;
    }
    if (v0 == taiiku_kari_rank[1]) {
        v1 = 1;
    }
    if (v0 == taiiku_kari_rank[2]) {
        v1 = 2;
    }
    if (v0 == taiiku_kari_rank[3]) {
        v1 = 3;
    }

    if (a1 != 0) {
        v1 = (v1 << 2) + (a0 << 4);
    } else {
        v1 = ((s32 *)p16)[v1];
        if (taiiku_konami_mode == 1 && v1 == 4) {
            v1 += buf44.w[taiiku_kari_gl_card[taiiku_kari_girl]];
        } else {
            v1 += buf44.w[((Rec68 *)&class_w[46].anim)[taiiku_kari_run_0_seat[a0][0]].f >> 3];
        }
    }
    return v1;
}
