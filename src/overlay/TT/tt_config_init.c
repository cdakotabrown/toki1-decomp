/* @CompileEnv: IDO52_COPYPROP_VAR=-12 IDO52_COPYPROP_SITE=lod:2:deny,lod:3:deny IDO52_UGEN_TEMP_REG=3:t9,4:t0,5:t1,6:t2,7:t3,8:t4,9:t5,10:t6,11:t6,12:t7,43:t9,44:t0,45:t1,46:t2,47:t3,48:t4 */
/* tt_config_init - TT overlay, Address: 0x8013E484, Size: 520 bytes. */
#include "types.h"
#include "tokimeki/save_work.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern s32 D_800F022C;
extern u16 tt_config;
extern u8 D_800F022B;
extern u8 tt_replay0_dat[];
extern u8 album_work[];

void tt_config_init(void)
{
    u8 *st;
    u8 *res;
    u8 *q;

    st = tt_game_wk;
    res = tt_step_wk;
    st[85] = 0;
    if ((D_800F022C << 6) < 0) {
        st[83] = 0;
    } else {
        st[83] = 1;
    }
    q = album_work;
    st[82] = ((u32)(*(s32 *)(q + 60) << 3)) >> 29;
    st[84] = ((u32)(*(s32 *)(q + 60) << 2)) >> 31;
    st[53] = st[59] = st[65] = 255;
    if (tt_config == 0) {
        *(u16 *)(st + 48) = 25544;
        st[50] = 20;
        st[51] = 36;
        st[52] = 24;
        *(u16 *)(st + 54) = 29144;
        st[56] = 23;
        st[57] = 36;
        st[58] = 10;
        *(u16 *)(st + 60) = 32744;
        st[62] = 22;
        st[63] = 36;
        st[64] = 18;
        st[68] = 7;
        st[69] = 6;
    } else {
        *(u16 *)(st + 48) = tt_config;
        st[50] = (*(TT_CONFIG *)&tt_config).name[0][0];
        st[51] = (*(TT_CONFIG *)&tt_config).name[0][1];
        st[52] = (*(TT_CONFIG *)&tt_config).name[0][2];
        *(u16 *)(st + 54) = (*(TT_CONFIG *)&tt_config).score[1];
        st[56] = (*(TT_CONFIG *)&tt_config).name[1][0];
        st[57] = (*(TT_CONFIG *)&tt_config).name[1][1];
        st[58] = (*(TT_CONFIG *)&tt_config).name[1][2];
        *(u16 *)(st + 60) = (*(TT_CONFIG *)&tt_config).score[2];
        st[62] = (*(TT_CONFIG *)&tt_config).name[2][0];
        st[63] = (*(TT_CONFIG *)&tt_config).name[2][1];
        st[64] = (*(TT_CONFIG *)&tt_config).name[2][2];
        st[68] = D_800F022B;
        st[69] = q[60];
        st[67] = q[61] & 1;
        if (st[83] == 0 && st[82] != 0) {
            st[73] = 0;
        } else {
            st[73] = ((u32)q[61] >> 1) & 1;
        }
    }
    if (st[83] == 0 && st[82] != 0) {
        res[34] = 1;
    } else {
        res[34] = 0;
    }
    *(u8 **)(st + 76) = tt_replay0_dat;
}
