/* Function: cb_read */
/* Address: 0x8008AC14 */
/* Size: 324 bytes (81 instructions) */
/* Region: Library (0x80086000-0x800BFFFF) */
/* Compiler: Psy-Q GCC 2.6.0 */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-all --fill-branch-store */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions 0xc 0x10 */
/* @PostProc: swap_instructions 0x60 0x64 */
/* @PostProc: swap_instructions 0xc8 0xd0 */
/* @PostProc: swap_instructions 0xcc 0xd4 */
/* @PostProc: swap_instructions 0xd0 0xd8 */
/* @PostProc: swap_instructions 0xd4 0xdc */
/* @PostProc: swap_instructions 0xf4 0xf8 */
/* @PostProc: swap_instructions 0xf8 0x100 */
extern int libcd_read_dest_ptr;
extern int libcd_read_sector_words;
extern volatile int libcd_read_sectors_left;
extern int libcd_read_saved_sync_cb;
extern int libcd_read_saved_ready_cb;
extern int CD_cbsync;
extern int CD_cbready;
extern int CD_cbread;

extern void CD_getsector(int, int);
extern void cd_read_retry(void);
extern void CD_cw(int, int, int, int);

void cb_read(int arg0, int arg1) {
    volatile int *p;
    volatile int *q;
    void (*callback)(int, int);

    if (arg0 == 1) {
        p = &libcd_read_sectors_left;
        if (*p > 0) {
            CD_getsector(libcd_read_dest_ptr, libcd_read_sector_words);
            libcd_read_dest_ptr = libcd_read_dest_ptr + (libcd_read_sector_words << 2);
            *p = *p - 1;
            (void)*p;
        }
    } else {
        q = &libcd_read_sectors_left;
        *q = -1;
    }

    p = &libcd_read_sectors_left;
    if (*p < 0) {
        cd_read_retry();
    }

    if (*p > 0) {
        return;
    }

    CD_cbready = 0;
    CD_cbsync = 0;
    CD_cw(9, 0, 0, 0);
    CD_cbsync = libcd_read_saved_sync_cb;
    CD_cbready = libcd_read_saved_ready_cb;

    callback = (void (*)(int, int))CD_cbread;
    if (callback != 0) {
        if (*p == 0) {
            callback(2, arg1);
        } else {
            callback(5, arg1);
        }
    }
}
