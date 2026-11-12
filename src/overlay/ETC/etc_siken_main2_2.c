/* @SymbolStart: 0x8014A828 */
/* @RodataBase: 0x8014D95C */
/* @SymbolSize: 252 */
/* etc_siken_main2_2 - ETC overlay, Address: 0x8014A830, Size: 244 bytes (61 insns) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 k_sub_reset();
extern s32 gnsx();
extern s32 sndisp();
extern s32 xa_wait();
extern char p_sei[];
extern u8 step_mode;
extern s32 etc_siken_sel;
extern char etc_siken_msg2_tbl[];
extern char etc_siken_kaijou_str[];

void etc_siken_main2_2(void)
{
    s32 t7;
    char *addr;

    switch (step_mode) {
    case 0:
        k_sub_reset();
        gnsx(p_sei);
        t7 = etc_siken_sel;
        addr = *(char **)(etc_siken_msg2_tbl + t7 * 4);
        sndisp(addr, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        gnsx(p_sei);
        sndisp(etc_siken_kaijou_str, 0, 15);
        step_mode = step_mode + 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        sub_sub_step_up();
        break;
    default:
        break;
    }
}
