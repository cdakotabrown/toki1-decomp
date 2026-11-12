/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:2 */
/* @SymbolStart: 0x801389DC */
/* bunka_sd_club5_cd_load - BUNKA_SD overlay, Address: 0x801389E4, Size: 248 bytes */
/* @SymbolSize: 256 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern void set_XAsector(s32, s32, s32);
extern s32 cd_read_start2(s32, s32, s32);

extern u8 step_mode;
extern u8 now_year;

s32 bunka_sd_club5_cd_load(void)
{
    u8 sel;
    s32 n;

    sel = step_mode;
    if (sel == 0) {
        cd_read_start2(86, 0x80180000, 20253);
        return step_mode = step_mode + 1;
    }
    if (sel == 1) {
        n = get_CD_State2();
        if (n & 1) {
            switch (now_year) {
            case 95:
                set_XAsector(63, 0x0500039C, 15);
                break;
            case 96:
                set_XAsector(0x0500005B, 0x05000450, 15);
                break;
            case 97:
            default:
                set_XAsector(0x02000062, 0x080003F6, 15);
                break;
            }
            device_cmd_send(768);
            sub_sub_step_up();
        }
    }
}
