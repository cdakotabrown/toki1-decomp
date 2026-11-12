/* Function: ot_buff_set */
/* Address: 0x80042498 */
/* Size: 268 bytes (67 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: swap_registers v1 a3 */
/* @PostProc: strip_trailing_instructions --count 1 --nop-only */
#include "types.h"

extern u8 now_step;
extern s32 activeBuff;
extern u8 cursor_flag;
extern u8 packet_buff_wide[];
extern u8 packet_buff[];
extern u8 ot_work[];

extern s32 GsGetActiveBuff(void);
extern void GsSetWorkBase(s32 a0);
extern void prim_buf_init(s32 a0, s32 a1);
extern void GsClearOt(s32 a0, s32 a1, s32 a2, s32 a3);

void ot_buff_set(void) {
    s32 val;
    s32 temp;

    val = GsGetActiveBuff();
    if (now_step == 0x90) {
        activeBuff = val;
        GsSetWorkBase((s32)(packet_buff_wide + val * 0x10C00));
        temp = activeBuff;
        prim_buf_init((s32)(packet_buff + temp * 0xA000), temp);
    } else {
        activeBuff = val;
        GsSetWorkBase((s32)(packet_buff + val * 0xA000));
        temp = activeBuff;
        prim_buf_init((s32)(packet_buff_wide + temp * 0x10C00), temp);
    }
    temp = activeBuff;
    GsClearOt(0, 0, (s32)(ot_work + temp * 0x14), temp);
    cursor_flag = 0;
}
