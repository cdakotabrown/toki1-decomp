/* Function: CD_shell */
/* Address: 0x8008A7BC */
/* Size: 152 bytes (38 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --konami-lib-sched */
/* @PostProc: strip_leading_instructions --count auto */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 CD_status;
extern s32 CD_status1;
extern s32 libcd_shell_open_seen;
extern s32 libcd_shell_open_msg;
extern void printf(s32 *);
extern void CD_cw(s32, s32, s32, s32);
extern void VSync(s32);

s32 CD_shell(void) {
    while (CD_status & 0x10) {
        printf(&libcd_shell_open_msg);
        CD_cw(1, 0, 0, 0);
        VSync(60);
    }
    if (CD_status1 != libcd_shell_open_seen) {
        CD_status1 = libcd_shell_open_seen;
        VSync(300);
    }
    return 0;
}
