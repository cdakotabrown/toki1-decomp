/* Address: 0x8008A854 */
/* Size: 536 bytes (134 instructions) */
/* Region: Library (GCC 2.6.0, Konami CD bring-up) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fno-schedule-insns --fill-load-delay --fill-earliest --reuse-at-same-hi --label-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0xb8:rt:a3:a0 0xbc:rs:a3:a0 0xbc:rt:a3:a0 0xd8:rt:a3:a0 0xd8:rd:a3:a0 0xe8:rs:a3:a0 0xe8:rd:a3:a0 0xf4:rs:a3:a0 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x30 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0xcc --offset2 0xdc */
/* @PostProc: swap_instructions --offset1 0xd0 --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xe4 */
/* @PostProc: swap_instructions --offset1 0xd8 --offset2 0xe8 */
/* @PostProc: swap_instructions --offset1 0xdc --offset2 0xe0 */
/* @PostProc: swap_instructions --offset1 0xe0 --offset2 0xec */
/* @PostProc: swap_instructions --offset1 0xe4 --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0xe8 --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0xec --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0xf0 --offset2 0xf4 */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x14c */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x140 --offset2 0x14c */
/* @PostProc: swap_instructions --offset1 0x144 --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x150 --offset2 0x154 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x15c */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x168 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x16c */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x198 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1b8 */
#include "types.h"

extern s32 VSync(s32 mode);
extern s32 CD_ready(s32 cmd, void *buf);
extern s32 CD_cw(s32 cmd, void *a, void *b, s32 c);
extern void CD_init(void);
extern void printf(const char *fmt, ...);

extern s32 libcd_timeout_deadline;
extern s32 libcd_timeout_spin_count;
extern const char *libcd_cur_func_name;
extern const char libcd_read_name_str[];
extern const char libcd_timeout_fmt[];

extern u8 libcd_last_cmd;
extern s32 cd_status;
extern s32 libcd_intr_code;
extern const char *CD_intstr[];
extern const char *CD_comstr[];

extern s32 CD_cbsync;
extern void (*CD_cbready)(void);
extern void cb_read(void);

extern u8 *libcd_reg_index_ptr;
extern u8 *libcd_reg_irq_ptr;
typedef struct { s32 mode; } VMode;
extern volatile VMode libcd_read_mode;
extern s32 libcd_read_dest_base;
typedef struct { s32 e0, pad[2]; s32 ec; } Blk;
extern Blk libcd_read_dest_ptr;
extern s32 libcd_read_sector_count;
extern volatile s32 libcd_read_sectors_left;
extern s32 libcd_read_timeout_vsync;

s32 cd_init_retry(void)
{
    u8 buf[8];
    u8 mode;
    s32 r;
    s32 v;
    s32 limit;
    s32 t;
    volatile s32 *res;
    u8 *p;
    u32 i0, i1, i2;

    libcd_timeout_deadline = VSync(-1) + 480;
    libcd_timeout_spin_count = 0;
    libcd_cur_func_name = libcd_read_name_str;

    res = &libcd_read_sectors_left;
    limit = 0xF00000;
    for (;;) {
        if (CD_ready(1, buf) == 0) {
            break;
        }
        if (libcd_timeout_deadline < VSync(-1) || libcd_timeout_spin_count++ > limit) {
            i0 = libcd_last_cmd; i1 = cd_status; i2 = libcd_intr_code;
            printf(libcd_timeout_fmt, libcd_cur_func_name, CD_comstr[i0],
                          CD_intstr[i1], CD_intstr[i2]);
            CD_init();
            r = -1;
        } else {
            r = 0;
        }
        if (r != 0) {
            *res = -1;
            return *res;
        }
    }

    CD_cbsync = 0;
    p = libcd_reg_index_ptr;
    CD_cbready = cb_read;
    *p = 0;
    *libcd_reg_irq_ptr = 0;
    mode = libcd_read_mode.mode;
    if (CD_cw(14, &mode, 0, 0) != 0) {
        libcd_read_sectors_left = -1;
        return libcd_read_sectors_left;
    }
    libcd_read_dest_ptr.e0 = libcd_read_dest_base;
    libcd_read_dest_ptr.ec = libcd_read_sector_count;
    if (CD_cw(6, 0, buf, 0) != 0) {
        libcd_read_sectors_left = -1;
        return libcd_read_sectors_left;
    }
    v = VSync(-1);
    t = libcd_read_sector_count + 480;
    libcd_read_timeout_vsync = v + t;
    return libcd_read_sectors_left;
}
