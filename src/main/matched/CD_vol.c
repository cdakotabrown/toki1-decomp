/* Function: CD_vol */
/* Address: 0x8008A51C */
/* Size: 168 bytes (42 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-epilogue */
/* @PostProc: scheduler_normalizer */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *libcd_reg_index_ptr;
extern u8 *libcd_reg_cmd_resp_ptr;
extern u8 *libcd_reg_param_ptr;
extern u8 *libcd_reg_irq_ptr;

extern s32 CD_sync(s32, s32);

s32 CD_vol(u8 *arg0) {
    CD_sync(0, 0);

    *libcd_reg_index_ptr = 2;
    *libcd_reg_param_ptr = arg0[0];
    *libcd_reg_irq_ptr = arg0[1];
    *libcd_reg_index_ptr = 3;
    *libcd_reg_cmd_resp_ptr = arg0[2];
    *libcd_reg_param_ptr = arg0[3];
    *libcd_reg_irq_ptr = 32;

    return 0;
}
