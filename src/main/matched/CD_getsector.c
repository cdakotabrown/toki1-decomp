/* Address: 0x8008B060 */
/* Size: 204 bytes (51 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern u8  *libcd_reg_index_ptr;
extern u8  *libcd_reg_irq_ptr;
extern u32 *libcd_getsector_cdrom_delay_ptr;
extern u32 *libcd_getsector_com_delay_ptr;
extern u32 *libcd_getsector_dpcr_ptr;
extern u32 *libcd_getsector_dma3_madr_ptr;
extern u32 *libcd_getsector_dma3_bcr_ptr;
extern u32 *libcd_getsector_dma3_chcr_ptr;

extern s32 CD_datasync(s32 arg0);

s32 CD_getsector(u32 addr, u32 block)
{
    CD_datasync(0);
    *libcd_reg_index_ptr = 0;
    *libcd_reg_irq_ptr = 128;
    *libcd_getsector_cdrom_delay_ptr = 0x20943;
    *libcd_getsector_com_delay_ptr = 4908;
    *libcd_getsector_dpcr_ptr |= 0x8000;
    *libcd_getsector_dma3_chcr_ptr = 0;
    *libcd_getsector_dma3_madr_ptr = addr;
    *libcd_getsector_dma3_bcr_ptr = block | 0x10000;
    *libcd_getsector_dma3_chcr_ptr = 0x11000000;
    return 0;
}
