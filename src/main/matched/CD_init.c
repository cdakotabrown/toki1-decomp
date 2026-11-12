/* Function: CD_init */
/* Address: 0x8008A5C4 */
/* Size: 504 bytes (126 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s32 cd_status;
extern s32 libcd_intr_code;
extern s32 libcd_dataend_event;
extern s32 CD_cbready;
extern s32 CD_cbsync;
extern s32 CD_status1;
extern s32 CD_status;
extern s32 CD_debug;
extern s32 libcd_read_state_blk[10];
extern u8 *libcd_reg_index_ptr;
extern u8 *libcd_reg_cmd_resp_ptr;
extern u8 *libcd_reg_param_ptr;
extern u8 *libcd_reg_irq_ptr;
extern s16 *libcd_init_spu_regs_ptr;
extern char libcd_state_ptr_tbl[32];
extern char libcd_init_addr_fmt[];

extern void printf(char *, char *);
extern void ResetCallback(void);
extern void InterruptCallback(s32, void *);
extern s32 CD_sync(s32, s32);
extern s32 CD_cw(s32, s32, s32, s32);
extern void callback(void);

s32 CD_init(void) {
    s32 i;
    s32 *p;
    u8 localBuf[4];

    printf(libcd_init_addr_fmt, libcd_state_ptr_tbl);

    cd_status = 2;
    libcd_intr_code = 0;
    libcd_dataend_event = 0;
    CD_cbready = 0;
    CD_cbsync = 0;
    CD_status1 = 0;
    CD_status = 0;
    CD_debug = 0;

    p = libcd_read_state_blk;
    for (i = 9; i != -1; i--) {
        *p++ = 0;
    }

    ResetCallback();
    InterruptCallback(2, (void *)callback);

    if (((u16 *)libcd_init_spu_regs_ptr)[220] == 0) {
        if (((u16 *)libcd_init_spu_regs_ptr)[221] == 0) {
            libcd_init_spu_regs_ptr[192] = 0x3FFF;
            libcd_init_spu_regs_ptr[193] = 0x3FFF;
        }
    }

    libcd_init_spu_regs_ptr[216] = 0x3FFF;
    libcd_init_spu_regs_ptr[217] = 0x3FFF;
    ((u16 *)libcd_init_spu_regs_ptr)[213] = 0xC001;

    localBuf[2] = 128;
    localBuf[3] = 0;
    localBuf[0] = 128;
    localBuf[1] = 0;

    CD_sync(0, 0);

    *libcd_reg_index_ptr = 2;
    *libcd_reg_param_ptr = localBuf[0];
    *libcd_reg_irq_ptr = localBuf[1];

    *libcd_reg_index_ptr = 3;
    *libcd_reg_cmd_resp_ptr = localBuf[2];
    *libcd_reg_param_ptr = localBuf[3];
    *libcd_reg_irq_ptr = 0x20;

    if (CD_cw(1, 0, 0, 0) != 0) {
        return -1;
    }
    if (CD_cw(10, 0, 0, 0) != 0) {
        return -1;
    }
    if (CD_cw(12, 0, 0, 0) != 0) {
        return -1;
    }

    return -(CD_sync(0, 0) != 2);
}
