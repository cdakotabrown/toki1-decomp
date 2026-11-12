/* Function: CD_readm */
/* Address: 0x8008AD58 */
/* Size: 204 bytes (51 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 --no-cross-jumping --fill-prev --reuse-at-same-hi */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 libcd_read_mode;
extern s32 libcd_read_sector_words;
extern s32 libcd_read_dest_base;
typedef struct { s32 a, b; } Pair;
extern Pair libcd_read_state_blk;
extern Pair libcd_read_saved_sync_cb;
extern s32 CD_cbsync;
extern s32 CD_cbready;

extern s32 cd_init_retry(void);
extern s32 cd_read_retry(void);

s32 CD_readm(s32 a0, s32 a1, s32 a2) {
    volatile s32 *p;
    volatile s32 *q;
    volatile s32 *r;
    s32 ret;
    s32 u;

    p = &libcd_read_mode;
    *p = a2;
    switch (*p & 0x30) {
    case 0:
        libcd_read_sector_words = 512;
        break;
    case 32:
        libcd_read_sector_words = 585;
        break;
    default:
        q = &libcd_read_sector_words;
        *q = 582;
        break;
    }
    p = &libcd_read_dest_base;
    *p = a0;
    libcd_read_state_blk.b = a1;
    libcd_read_state_blk.a = 16;
    u = CD_cbready;
    libcd_read_saved_sync_cb.a = CD_cbsync;
    libcd_read_saved_sync_cb.b = u;
    ret = cd_init_retry();
    if (ret < 0) {
        ret = cd_read_retry();
        return -(ret < 1);
    }
    return 0;
}
