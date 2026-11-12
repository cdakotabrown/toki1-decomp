/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 IDO52_UGEN_TREG_ORDER=t7,t8,t9,t0,t1,t2,t3,t4,t5,t6 */
/* @SymbolStart: 0x801406E4 */
/* @SymbolSize: 280 */
#include "types.h"

typedef struct {
    s32 a0;
    s32 a1;
} PairEntry;

extern s32 set_XAsector();
extern s32 device_cmd_send();
extern s32 printf();
extern s32 get_CD_State();
extern s32 card_state_clear();
extern s32 goto_sub_step();
extern s32 goto_sub_sub_step();
extern s32 save_flag_clear();

extern u8 now_album;
extern u8 step_mode;
extern PairEntry name_ent_xa_tbl[];
extern u8 name_ent_mainff_str0[];

s32 name_ent_mainff()
{
    s32 ret;
    u32 idx;

    ret = step_mode;
    switch (step_mode) {
    case 0:
        idx = now_album - 1;
        set_XAsector(name_ent_xa_tbl[idx].a0, name_ent_xa_tbl[idx].a1, idx % 13);
        device_cmd_send(768);
        ret = printf(name_ent_mainff_str0);
        step_mode = step_mode + 1;
        break;
    case 1:
        ret = get_CD_State();
        if (ret & 0x0004) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        ret = get_CD_State();
        if (ret & 0x0002) {
            step_mode = step_mode + 1;
        }
        break;
    case 3:
        card_state_clear();
        save_flag_clear();
        ret = goto_sub_step(1);
        break;
    }
    return ret;
}
