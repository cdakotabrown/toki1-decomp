/* Address: 0x8013BEF0 */
/* Size: 68 bytes (17 insns) */
#include "types.h"
#include "tokimeki/etc_cal_cw_res.h"
extern ETC_CAL_CW_RES etc_cal_cw_res;


void etc_buf_init(void) {
    etc_cal_cw_res.hdr = 0x8019DCFC;
    etc_cal_cw_res.img = 0x8019DD00;
    etc_cal_cw_res.anim_tbl = 0x8019DDC0;
    etc_cal_cw_res.aux = *(s16 *)0x8019DDF8;
}
