/* Function: normal_date_bg_in_sub */
/* Address: 0x8008B060 */
/* Size: 204 bytes (51 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-all */
/* @PostProc: custom_script custom_func_8008B060.py */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 *libcd_reg_index_ptr;
extern s32 *libcd_reg_irq_ptr;
extern s32 *libcd_getsector_cdrom_delay_ptr;
extern s32 *libcd_getsector_com_delay_ptr;
extern s32 *libcd_getsector_dpcr_ptr;
extern s32 *libcd_getsector_dma3_madr_ptr;
extern s32 *libcd_getsector_dma3_bcr_ptr;
extern s32 *libcd_getsector_dma3_chcr_ptr;

extern void CD_datasync(s32);

s32 normal_date_bg_in_sub(s32 arg0, s32 arg1) {
    CD_datasync(0);
    *(s8 *)libcd_reg_index_ptr = 0;
    *(u8 *)libcd_reg_irq_ptr = 128;
    *libcd_getsector_cdrom_delay_ptr = 0x20943;
    *libcd_getsector_com_delay_ptr = 0x132C;
    *libcd_getsector_dpcr_ptr |= 0x8000;
    *libcd_getsector_dma3_chcr_ptr = 0;
    *libcd_getsector_dma3_madr_ptr = arg0;
    *libcd_getsector_dma3_bcr_ptr = arg1 | 0x10000;
    *libcd_getsector_dma3_chcr_ptr = 0x11000000;
    return 0;
}
