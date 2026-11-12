/* Function: callback */
/* Address: 0x80089900 */
/* Size: 200 bytes (50 instructions) */
/* Region: Library (0x80086000+) */
/* Likely Symbol: callback */
/* @Compiler: gcc260 */
/* @CompileFlags: --konami-lib-sched */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef void (*SdFunc)(s32, void *);

extern u8 *libcd_reg_index_ptr;
extern s32 cd_status;
extern s32 libcd_intr_code;
extern char Result;
extern char libcd_sync_result_buf;
extern SdFunc CD_cbsync;
extern SdFunc CD_cbready;

extern s32 getintr(void);

void callback(void) {
    s32 result;
    u8 saved;

    saved = *libcd_reg_index_ptr & 3;
    for (;;) {
        result = getintr();
        if (result == 0) {
            break;
        }
        if (result & 4) {
            if (CD_cbready) {
                CD_cbready(libcd_intr_code, &Result);
            }
        }
        if (result & 2) {
            if (CD_cbsync) {
                CD_cbsync(cd_status, &libcd_sync_result_buf);
            }
        }
    }
    *libcd_reg_index_ptr = saved;
}
