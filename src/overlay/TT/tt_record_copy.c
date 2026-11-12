/* @SymbolStart: 0x80134D0C */
/* @SymbolSize: 276 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-20@8 IDO52_FORCE_WEB_COLOR=22:a2,14:a1,0:v1,17:t0,39:a3,70:t2,69:t1 */
/* tt_record_copy - TT overlay, Address: 0x80134D0C (head absorbed), Size: 276 bytes (69 insns) */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_scale_buf;
extern u8 *tt_db_buf;

typedef struct { u32 addr : 24; u32 len : 8; } P_TAG;
typedef struct { u8 pad[0x8000]; P_TAG ot[46]; } DB;
extern void tt_sprite_ot_add_var(u8 *arg0);

void tt_record_copy(u8 *rec)
{
    u8 *db;
    u8 *p;
    u8 *st;
    u8 *dst;
    u8 *p2;
    u8 *rec2;

    db = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    p = *(u8 **)(db + 4);
    p2 = p;
    st = tt_game_wk;
    dst = tt_scale_buf;
    *(u8 **)(db + 4) = p + 48;
    *(s16 *)(dst + 8) = *(s16 *)(rec + 20);
    *(s16 *)(dst + 10) = *(s16 *)(rec + 22);
    *(s32 *)(dst + 12) = *(u16 *)(rec + 96) + 2048;
    rec2 = rec;
    *(s16 *)(p + 8) = *(s16 *)(rec + 20) - 12;
    *(s16 *)(p + 10) = *(s16 *)(rec + 22) - 12;
    rec = 0;
    *(u8 *)(p + 41) = 0;
    *(u8 *)(p + 42) = 1;
    *(s16 *)(p + 16) = 24;
    *(s16 *)(p + 18) = 24;
    *(u8 *)(p + 40) = 0;
    *(s16 *)(p + 22) = 11;
    if (*(u16 *)(st + 8) & 1) {
        *(u8 *)(p + 12) = 224;
    } else {
        *(u8 *)(p2 + 12) = 176;
    }
    p = 0;
    *(u8 *)(p2 + 13) = 0;
    *(s16 *)(p2 + 14) = 15764;
    *(u8 *)(p2 + 43) = 0;
    *(u8 *)(p2 + 44) = 1;
    tt_sprite_ot_add_var((u8 *)&((DB *)db)->ot[*(u16 *)(rec2 + 16) + 2]);
}
