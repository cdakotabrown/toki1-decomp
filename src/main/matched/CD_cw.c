/* Function: CD_cw */
/* Address: 0x8008A1F8 */
/* Size: 804 bytes (201 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC 2.7.2 */
/* @Compiler: gcc272 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-load-delay --fill-earliest --load-dest-temp */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0xc8 --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xd4 --offset2 0xd8 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x170 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x184 --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1bc --offset2 0x1c0 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1c4 */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1d8 --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1e8 */
/* @PostProc: swap_instructions --offset1 0x240 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x248 */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x24c */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x25c */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x260 */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x260 */
/* @PostProc: swap_instructions --offset1 0x258 --offset2 0x264 */
/* @PostProc: swap_instructions --offset1 0x25c --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x268 --offset2 0x26c */
/* @PostProc: patch_insn_reg_field 0x230:rt:a3:a0 0x234:rs:a3:a0 0x234:rt:a3:a0 0x240:rd:a3:a0 0x240:rt:a3:a0 0x250:rd:a3:a0 0x250:rs:a3:a0 0x25c:rs:a3:a0 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 VSync(s32);
extern void printf(const char *, ...);
extern void CD_init(void);
extern s32 CheckCallback(void);
extern void callback(void);
extern s32 CD_sync(s32, s32);

extern s32 CD_debug;
extern s32 CD_comstr[];
extern s32 CD_intstr[];
extern s32 libcd_cmd_param_count_tbl[];
extern s32 libcd_cmd_reset_ready_tbl[];
extern volatile s32 cd_status;
extern volatile s32 libcd_intr_code;
extern u8 *libcd_reg_index_ptr;
extern u8 libcd_setloc_msf_buf[];
extern volatile u8 libcd_last_cmd;
extern u8 *libcd_reg_param_ptr;
extern volatile u8 * volatile libcd_reg_cmd_resp_ptr;

extern volatile s32 libcd_timeout_deadline;
extern volatile s32 libcd_timeout_spin_count;
extern void *libcd_cur_func_name;
extern char libcd_sync_result_buf;
extern char libcd_timeout_fmt;
extern char libcd_cmd_trace_fmt[];
extern char libcd_cmd_noparam_fmt[];
extern char libcd_cw_name_str;

s32 CD_cw(s32 mode, u8 *data, void *outbuf, s32 immediate) {
    s32 i;
    s32 time2;
    s32 count;
    s32 result;
    s32 *base_2b8c;
    s32 *base_2b0c;
    s32 pad1;
    s32 limit;
    s32 pad2;

    (void)&pad1;
    (void)&pad2;

    if (CD_debug >= 2) {
        printf(libcd_cmd_trace_fmt, CD_comstr[(u8)mode]);
    }

    if (libcd_cmd_param_count_tbl[(u8)mode] != 0) {
        if (data == 0) {
            if (CD_debug > 0) {
                printf(libcd_cmd_noparam_fmt, CD_comstr[(u8)mode]);
            }
            return -2;
        }
    }

    if ((u8)mode == 2) {
        for (i = 0; i < 4; i++) {
            libcd_setloc_msf_buf[i] = data[i];
        }
    }

    CD_sync(0, 0);

    cd_status = 0;
    if (libcd_cmd_reset_ready_tbl[(u8)mode] != 0) {
        libcd_intr_code = 0;
    }

    *libcd_reg_index_ptr = 0;

    i = 0;
    if (libcd_cmd_param_count_tbl[(u8)mode] > 0) {
        do {
            *libcd_reg_param_ptr = *data++;
            i++;
        } while (i < libcd_cmd_param_count_tbl[(u8)mode]);
    }

    libcd_last_cmd = (u8)mode;
    *libcd_reg_cmd_resp_ptr = libcd_last_cmd;

    if (immediate != 0) {
        return 0;
    }

    libcd_timeout_deadline = VSync(-1) + 480;
    libcd_timeout_spin_count = 0;
    libcd_cur_func_name = &libcd_cw_name_str;

    if (cd_status != 0) {
        goto post_loop;
    }
    limit = 0x00F00000;
    base_2b8c = CD_intstr;
    base_2b0c = CD_comstr;

    do {
        time2 = VSync(-1);
        if (libcd_timeout_deadline < time2 || libcd_timeout_spin_count++ > limit) {
            printf(&libcd_timeout_fmt, libcd_cur_func_name,
                          base_2b0c[libcd_last_cmd],
                          base_2b8c[cd_status],
                          base_2b8c[libcd_intr_code]);
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
    } while (cd_status == 0);

post_loop:
    {
        char *dst = (char *)outbuf;
        char *src = &libcd_sync_result_buf;
        s32 n;
        if (dst != 0) {
            n = 7;
            while (n != -1) {
                *dst++ = *src++;
                n--;
            }
        }
    }

    {
        s32 dc8 = cd_status;
        return -(dc8 == 5);
    }
}
