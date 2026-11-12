/* @SymbolStart: 0x8013753C */
/* @SymbolSize: 260 */
/* Address: 0x8013753C (head) / 0x80137544 */
/* Size: 260 bytes (65 insns) */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @CompileEnv: IDO52_FORCE_WEB_SPILL=2 IDO52_UGEN_TEMP_REG=1:a0 IDO52_UGEN_TEMP_REG_INPLACE=occ:1 */
#include "types.h"
#include "tokimeki/class_w.h"

extern s16 cal_cw_x;
extern s32 load_done_flag;
extern s16 taiiku_kari_scrl_hi;
extern s16 class_w[];

extern s32 device_cmd_send(s32);

void taiiku_item_pickup(void) {
    s32 a0 = taiiku_kari_scrl_hi;
    s16 *v0;
    s16 *v1;
    s32 v0_y;

    if (a0 >= 961) {
        return;
    }

    if ((s32)(cal_cw_x + 16) >= (s32)((CLASS_W *)class_w)[100].x.half[1]) {
        if (((CLASS_W *)class_w)[100].anim_flag != 0) {
            if (load_done_flag == 1) {
                device_cmd_send(1545);
            } else {
                device_cmd_send(1289);
            }
            ((CLASS_W *)class_w)[100].anim_flag = 0;
            ((CLASS_W *)class_w)[100].disp_flag = 0;
            a0 = taiiku_kari_scrl_hi;
        }
    }

    v0 = class_w;
    v1 = &(*(s16 *)&((CLASS_W *)class_w)[3]);
    do {
        if ((s32)v0[3317] >= (s32)v0[3453]) {
            ((u8 *)v0)[6870] = 0;
            ((u8 *)v0)[6871] = 0;
        }
        v0 = (s16 *)((char *)v0 + 68);
    } while ((u32)v0 < (u32)v1);

    v0_y = 232 - a0 * 2;
    ((CLASS_W *)class_w)[100].x.half[1] = (s16)v0_y;
    ((CLASS_W *)class_w)[101].x.half[1] = (s16)v0_y;
    ((CLASS_W *)class_w)[102].x.half[1] = (s16)v0_y;
    ((CLASS_W *)class_w)[103].x.half[1] = (s16)v0_y;
}
