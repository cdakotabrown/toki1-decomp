/* Function: cd_read_retry */
/* Address: 0x8008AA6C */
/* Size: 424 bytes (106 instructions) */
/* Region: Library (0x80086000+), Psy-Q GCC */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --label-load-nop --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x17c:rt:v1:v0 0x180:rt:v1:v0 0x180:rs:v1:v0 0x184:rt:v0:v1 0x188:rt:v0:v1 0x188:rs:v1:v0 0x18c:rs:v1:v0 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 CD_status;
extern s32 CD_status1;
extern s32 libcd_shell_open_seen;
extern s32 libcd_retry_msg_busy;
extern volatile s32 libcd_read_state_blk[];
extern volatile s32 libcd_read_sectors_left[];
extern u8 libcd_setloc_msf_buf[];
extern s32 libcd_shell_open_msg;
extern s32 cd_read_retry_tbl0;

extern void printf();
extern void VSync(s32);
extern s32 CD_cw(s32, s32, s32, s32);
extern s32 cd_init_retry(void);

s32 cd_read_retry(void) {
    volatile s32 *peakPtr;
    u8 *peakData;
    s32 val;

    if (libcd_read_state_blk[0] == 0) {
        goto exit_path;
    }
    peakData = libcd_setloc_msf_buf;
    peakPtr = libcd_read_state_blk;

    do {
        if (libcd_read_sectors_left[0] >= 0) {
            goto exit_path;
        }
        while (CD_status & 0x10) {
            printf(&libcd_shell_open_msg);
            CD_cw(1, 0, 0, 0);
            VSync(60);
        }
        if (CD_status1 != libcd_shell_open_seen) {
            CD_status1 = libcd_shell_open_seen;
            VSync(300);
        }
        if (libcd_retry_msg_busy == 0) {
            libcd_retry_msg_busy = 1;
            printf(&cd_read_retry_tbl0, *peakPtr, peakData[0], peakData[1],
                          peakData[2]);
            libcd_retry_msg_busy = 0;
        }
        *peakPtr = *peakPtr - 1;
        val = *peakPtr;
        if (CD_cw(8, 0, 0, 0) == 0) {
            if (CD_cw(2, (s32)libcd_setloc_msf_buf, 0, 0) == 0) {
                if (cd_init_retry() >= 0) {
                    return peakPtr[6];
                }
            }
        }
    } while (*peakPtr != 0);

exit_path:
    libcd_read_sectors_left[0] = -1;
    return libcd_read_sectors_left[0];
}
