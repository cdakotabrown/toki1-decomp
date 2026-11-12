/* @CompileEnv: IDO52_AS1_NO_COPY_PROP=0,1 IDO52_FORCE_WEB_COLOR=0:a1 IDO52_AS1_TIE_REJECT=50,397 IDO52_UGEN_TEMP_REG=19:t4,20:t3,21:t5,22:t6,23:t7,24:t8,25:t1 */
/* @SymbolStart: 0x801570B4 */
/* @SymbolSize: 324 */
/* @PostProc: patch_insn_reg_field 0xec:rd:t2:t1 0xf0:rd:t5:t2 0xf0:rt:t2:t1 0xf4:rt:t5:t2 */
#include "types.h"

typedef struct {
    u8 pad0[116];
    s16 f116;
    s16 f118;
    s16 f120;
    s16 f122;
    u8 pad2[12];
} Rec;

typedef struct {
    s16 a;
    s16 b;
    s16 c;
    s16 d;
} Blk8;

extern Rec *taco_actor_rec;

extern s32 rsin();
extern void taco_shot_spawn();
extern void taco_actor_exit();

#define REC (idx + taco_actor_rec)
void
taco_actor_step10(s32 idx)
{
    Blk8 s;
    s32 h;

    h = REC->f122;
    if (h % 60 == 0) {
        s.a = REC->f116;
        s.b = REC->f118;
        s.c = REC->f120;
        taco_shot_spawn(s, 128, 0);
        h = REC->f122;
    }
    h = rsin(h << 6) * 5;
    h = h << 2;
    REC->f118 = REC->f118 + (s16)(h >> 12);
    REC->f120 = REC->f120 + 150;
    if (REC->f120 < 4097) {
        return;
    }
    taco_actor_exit(idx);
}
