/* @SymbolSize: 248 */
/* Address: 0x8013BDEC */
/* Size: 248 bytes (62 instructions) */
#include "types.h"

extern s32 bustup_cd_read(s32);
extern s32 etc_bustup_flag;
extern s32 etc_bustup_season_tbl[];
extern s32 etc_bustup_season_tbl_1_0[];
extern s32 etc_bustup_season_tbl_2_0[];
extern s32 etc_bustup_season_tbl_3_0[];

void etc_bustup_season_read(s32 a0, s32 a1)
{
    s32 v0;

    switch (a1) {
    case 0:
        v0 = etc_bustup_season_tbl[a0];
        if (v0 == -1) {
            return;
        }
        etc_bustup_flag = 1;
        bustup_cd_read(v0);
        return;
    case 1:
        v0 = etc_bustup_season_tbl_1_0[a0];
        if (v0 == -1) {
            return;
        }
        etc_bustup_flag = 1;
        bustup_cd_read(v0);
        return;
    case 2:
        v0 = etc_bustup_season_tbl_2_0[a0];
        if (v0 == -1) {
            return;
        }
        etc_bustup_flag = 1;
        bustup_cd_read(v0);
        return;
    case 3:
        v0 = etc_bustup_season_tbl_3_0[a0];
        if (v0 == -1) {
            return;
        }
        etc_bustup_flag = 1;
        bustup_cd_read(v0);
        return;
    }
}
