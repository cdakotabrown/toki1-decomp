/* @SymbolStart: 0x8014A4B4 */
/* @RodataBase: 0x8014D8FC */
/* Address: 0x8014A4BC */
/* Size: 408 bytes (102 insns) */
/* @SymbolSize: 416 */
#include "types.h"

extern void k_sub_reset(void);
extern void gnsx(s32);
extern s32  get_etc_index(void);
extern void sndisp(s32, s32, s32);
extern void device_port_init(s32, s32);
extern void xa_wait(void);
extern void etc_flag_set(void);
extern void goto_sub_step(s32);

extern u8  p_sei[];
extern u8  step_mode;
extern s32 etc_sound7_msg_tbl[];
extern u8  etc_sound_main7_str0[];

void etc_sound_main7(void) {
    s32 idx = step_mode;

    switch (idx) {
    case 0:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp(etc_sound7_msg_tbl[get_etc_index()], 0, 31);
        switch (get_etc_index()) {
        case 0: device_port_init(1, 516); break;
        case 1: device_port_init(1, 513); break;
        case 2: device_port_init(1, 513); break;
        case 3: device_port_init(1, 512); break;
        case 4: device_port_init(1, 515); break;
        case 5: device_port_init(1, 515); break;
        }
        step_mode++;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        gnsx((s32)p_sei);
        sndisp((s32)etc_sound_main7_str0, 0, 31);
        step_mode++;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        etc_flag_set();
        goto_sub_step(4);
        break;
    }
}
