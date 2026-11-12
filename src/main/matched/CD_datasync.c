/* Function: CD_datasync */
/* Address: 0x8008AF08 */
/* Size: 344 bytes (86 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -mhard-float -fno-schedule-insns --fill-earliest --fill-load-delay */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xa8:rt:a2:v1 0xac:rs:a2:v1 0xac:rt:a2:v1 0xb0:rt:v1:a0 0xb4:rs:v1:a0 0xb4:rt:v1:a0 0xc0:rt:a2:v1 0xc0:rd:a2:v1 0xc4:rs:a2:v1 0xc4:rd:a2:v1 0xc8:rt:v1:a0 0xc8:rd:v1:a0 0xd8:rs:v1:a0 0xd8:rd:v1:a0 0xe0:rs:a2:v1 0xe4:rs:v1:a0 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa8 */
/* @PostProc: swap_instructions --offset1 0xa4 --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc8 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xe4 */
typedef unsigned char u8;
typedef signed int s32;

extern s32 VSync(s32);
extern void printf(char *fmt, ...);
extern void CD_init(void);

extern s32 libcd_timeout_deadline;
extern s32 libcd_timeout_spin_count;
extern char *libcd_cur_func_name;
extern char libcd_datasync_name_str[];
extern char libcd_timeout_fmt[];

extern u8 libcd_last_cmd;
extern s32 cd_status;
extern s32 libcd_intr_code;
extern s32 CD_comstr[];
extern s32 CD_intstr[];
extern s32 *libcd_getsector_dma3_chcr_ptr;

s32 CD_datasync(s32 arg0)
{
    s32 result;

    libcd_timeout_deadline = VSync(-1) + 480;
    libcd_timeout_spin_count = 0;
    libcd_cur_func_name = libcd_datasync_name_str;
    do {
        if (libcd_timeout_deadline < VSync(-1) || libcd_timeout_spin_count++ > 0xF00000) {
            printf(libcd_timeout_fmt, libcd_cur_func_name, CD_comstr[libcd_last_cmd],
                          CD_intstr[cd_status], CD_intstr[libcd_intr_code]);
            CD_init();
            result = -1;
        } else {
            result = 0;
        }
        if (result != 0) {
            return -1;
        }
        if (!(*libcd_getsector_dma3_chcr_ptr & 0x1000000)) {
            return 0;
        }
        if (arg0 != 0) {
            break;
        }
    } while (1);
    return 1;
}
