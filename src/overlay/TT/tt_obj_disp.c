/* @SymbolSize: 620 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=4,32,101,113 IDO52_FORCE_WEB_COLOR=45:v1,76:a0 */
/* tt_obj_disp - TT overlay, Address: 0x801336F8, Size: 620 bytes (155 insns) */
#include "types.h"

typedef struct {
    u32 addr : 24;
    u32 len : 8;
} P_TAG;

extern u8 *tt_game_wk;
extern u8 *tt_disp_wk;
extern u8 *tt_obj_pool;
extern u8 *tt_db_buf;

extern void anim_sprite_disp(u8 *dst, u8 *src, s32 n);
typedef struct { u8 pad[0x8000]; P_TAG ot[46]; } DB;
#define OT2(db, s) (((DB *)(db))->ot[*(u16 *)((s) + 16) + 2])
extern void SetDrawMode(u8 *p, s32 a, s32 b, s32 c, s32 d);

void tt_obj_disp(void)
{
    u8 *g;
    u8 *s;
    u8 *db;
    s32 i;
    u8 *prim;
    u8 *q;

    db = tt_db_buf + *(s32 *)(tt_disp_wk + 8) * 32952;
    g = tt_game_wk;
    s = tt_obj_pool;

    for (i = 0; i != 32; i++, s += 120) {
        if (s[0] == 0) {
            continue;
        }
        if ((*(u16 *)(s + 86) & 2) != 0) {
            if ((((u32)*(u16 *)(g + 8) >> 1) & 1) != 0) {
                continue;
            }
        }
        if (*(u16 *)(s + 4) == 113) {
            anim_sprite_disp(s + 20, s + 56, *(u16 *)(s + 16));
        } else {
            prim = *(u8 **)(db + 4);
            *(u8 **)(db + 4) = prim + 20;
            prim[3] = 4; prim[7] = 101;
            *(s16 *)(prim + 8) = (s16)(*(s16 *)(s + 20) - 8);
            *(s16 *)(prim + 10) = (s16)(*(s16 *)(s + 22) - 8);
            *(s16 *)(prim + 18) = 16;
            *(s16 *)(prim + 16) = 16;
            prim[12] = s[92];
            prim[13] = s[93];
            *(s16 *)(prim + 14) = (s16)*(u16 *)(s + 90);

            ((P_TAG *)prim)->addr = OT2(db, s).addr, OT2(db, s).addr = (u32)prim;

            q = *(u8 **)(db + 4);
            *(u8 **)(db + 4) += 12;
            SetDrawMode(q, 0, 0, *(u16 *)(s + 88), 0);

            ((P_TAG *)q)->addr = OT2(db, s).addr, OT2(db, s).addr = (u32)q;
        }
    }
}
