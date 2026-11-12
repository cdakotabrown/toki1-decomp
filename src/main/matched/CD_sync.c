/* Function: CD_sync */
/* Address: 0x80089E50 */
/* Size: 436 bytes (109 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-earliest --fill-load-delay -fno-schedule-insns */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xb0:rt:a2:v1 0xb4:rs:a2:v1 0xb4:rt:a2:v1 0xb8:rt:v1:a0 0xbc:rs:v1:a0 0xbc:rt:v1:a0 0xc8:rt:a2:v1 0xc8:rd:a2:v1 0xcc:rs:a2:v1 0xcc:rd:a2:v1 0xd0:rt:v1:a0 0xd0:rd:v1:a0 0xe0:rs:v1:a0 0xe0:rd:v1:a0 0xe8:rs:a2:v1 0xec:rs:v1:a0 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0x60 */
/* @PostProc: swap_instructions --offset1 0xa8 --offset2 0xb0 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb4 --offset2 0xbc */
/* @PostProc: swap_instructions --offset1 0xb8 --offset2 0xc0 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xd0 */
/* @PostProc: swap_instructions --offset1 0xc4 --offset2 0xd4 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe8 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x154 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 VSync(s32);
extern void printf(void *, void *, s32, s32, s32);
extern void CD_init(void);
extern s32 CheckCallback(void);
extern void callback(void);

extern s32 libcd_timeout_deadline;
extern s32 libcd_timeout_spin_count;
extern void *libcd_cur_func_name;
extern u8 libcd_sync_name_str;
extern void *libcd_timeout_fmt;

extern u8 libcd_last_cmd;
extern volatile s32 cd_status;
extern s32 libcd_intr_code;
extern s32 CD_comstr[];
extern s32 CD_intstr[];
extern char libcd_sync_result_buf;

s32 CD_sync(s32 arg0, s32 arg1) {
    s32 result;
    s32 dc8;
    s32 i;
    char *src;
    char *dst;
    s32 *b8c;
    s32 *b0c;

    libcd_timeout_deadline = VSync(-1) + 480;
    libcd_timeout_spin_count = 0;
    libcd_cur_func_name = &libcd_sync_name_str;
    b8c = CD_intstr;
    b0c = CD_comstr;
    do {
        if (libcd_timeout_deadline < VSync(-1) || libcd_timeout_spin_count++ > 0xF00000) {
            printf(&libcd_timeout_fmt, libcd_cur_func_name, b0c[libcd_last_cmd],
                          b8c[cd_status], b8c[libcd_intr_code]);
            CD_init();
            result = -1;
        } else {
            result = 0;
        }
        if (result != 0) {
            return -1;
        }
        if (CheckCallback() != 0) {
            callback();
        }
        dc8 = cd_status;
        if (dc8 == 2 || dc8 == 5) {
            src = &libcd_sync_result_buf;
            cd_status = 2;
            dst = (char *)arg1;
            if (dst != 0) {
                for (i = 7; i != -1; i--) {
                    *dst++ = *src++;
                }
            }
            return dc8;
        }
    } while (arg0 == 0);
    return 0;
}
