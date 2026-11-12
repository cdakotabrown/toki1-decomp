/* Function: getintr */
/* Address: 0x800899C8 */
/* Size: 1160 bytes (290 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --load-dest-temp --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 *libcd_reg_index_ptr;
extern u8 *libcd_reg_cmd_resp_ptr;
extern u8 *libcd_reg_param_ptr;
extern u8 *libcd_reg_irq_ptr;
extern u8 libcd_last_cmd;
extern s32 libcd_cmd_stat_reply_tbl[];
extern s32 CD_status;
extern s32 CD_status1;
extern s32 CD_comstr[];
extern s32 libcd_cmd_second_reply_tbl[];
extern volatile s32 cd_status;
extern volatile s32 libcd_intr_code;
extern volatile s32 libcd_dataend_event;
extern u8 Result[8];
extern u8 libcd_sync_result_buf[8];
extern u8 libcd_dataend_result_buf[8];
extern char getintr_tbl0[];
extern char getintr_tbl1[];

extern void printf();

static __inline__ void copy8(u8 *d, u8 *s)
{
    s32 n;

    if (d != 0) {
        for (n = 7; n != -1; n--) {
            *d = *s;
            d++;
            s++;
        }
    }
}

#define COPY8(SRC, DST) copy8((DST), (SRC))

s32 getintr(void)
{
    u8 buf[8];
    s32 i;
    s32 mode;
    s32 cls;
    s32 idx;

    *libcd_reg_index_ptr = 1;
    mode = (*libcd_reg_irq_ptr) & 7;
    if (mode == 0) {
        return 0;
    }

    cls = 0;
    for (i = 0; i < 8; i++) {
        if ((*libcd_reg_index_ptr & 0x20) == 0) {
            break;
        }
        buf[i] = *libcd_reg_cmd_resp_ptr;
    }
    for (; i < 8; i++) {
        buf[i] = 0;
    }

    *libcd_reg_index_ptr = 1;
    *libcd_reg_irq_ptr = 7;
    *libcd_reg_param_ptr = 7;

    if (libcd_cmd_stat_reply_tbl[libcd_last_cmd] != 0) {
        if (!(CD_status & 0x10) && (buf[0] & 0x10)) {
            CD_status1 = CD_status1 + 1;
        }
        {
            u8 b0 = buf[0];
            CD_status = b0;
            cls = b0 & 0x1C;
        }
    }

    if (cls != 0 || mode == 5) {
        printf(getintr_tbl0, CD_comstr[libcd_last_cmd], buf[0]);
    }

    idx = mode - 1;
    if ((u32)idx < 5) {
    switch (idx) {
    case 2:

        if (cls != 0) {
            cd_status = 5;
            COPY8(buf, libcd_sync_result_buf);
            return 2;
        }
        if (cd_status == 0 && libcd_cmd_second_reply_tbl[libcd_last_cmd] != 0) {
            cd_status = 3;
            COPY8(buf, libcd_sync_result_buf);
            return 1;
        }
        cd_status = 2;
        COPY8(buf, libcd_sync_result_buf);
        return 2;

    case 1:
        cd_status = (cls != 0) ? 5 : 2;
        COPY8(buf, libcd_sync_result_buf);
        return 2;

    case 0:
        libcd_intr_code = (cls != 0) ? 5 : 1;
        COPY8(buf, Result);
        return 4;

    case 3:
        libcd_dataend_event = 4;
        libcd_intr_code = libcd_dataend_event;
        COPY8(buf, libcd_dataend_result_buf);
        COPY8(buf, Result);
        return 4;

    case 4:
        libcd_intr_code = 5;
        cd_status = libcd_intr_code;
        COPY8(buf, libcd_sync_result_buf);
        COPY8(buf, Result);
        return 6;
    }
    }

    printf(getintr_tbl1, mode);
    return -1;
}
