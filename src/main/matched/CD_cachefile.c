/* @Compiler: gcc272 */
/* @CompileFlags: -O2 -G0 -mips1 --fill-earliest --fill-load-delay --load-dest-temp --macro-lui-fill --jal-load-nop */
/* Function: CD_cachefile */
/* Address: 0x800895F0 */
/* Size: 672 bytes (168 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x8 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0xc */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x18 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x3c */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x40 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x118 --offset2 0x11c */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x120 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x164 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1bc */
/* @PostProc: swap_instructions --offset1 0x1b8 --offset2 0x1c8 */
/* @PostProc: swap_instructions --offset1 0x1bc --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1c0 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1cc --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1e0 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1dc --offset2 0x1e0 */
/* @PostProc: swap_instructions --offset1 0x1f4 --offset2 0x1f8 */
/* @PostProc: swap_instructions --offset1 0x1fc --offset2 0x200 */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x204 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

extern s32 libcd_cached_dir_id;
extern s32 CD_debug;
extern u8 load_buf[];
extern u8 libcd_cached_file_tbl[];
extern char libcd_cachefile_nodir_msg[];
extern char libcd_cachefile_search_msg[];
extern char libcd_cachefile_entry_fmt[];
extern char libcd_cachefile_count_fmt[];

extern u16 libcd_dot_name_str;
extern s16 libcd_dotdot_name_str;
extern s8 libcd_dotdot_name_nul;

typedef struct {
    s32 field_00;
    u8 pad04[40];
} Entry44;

extern Entry44 libcd_dir_ent_lba_view[];

s32 cd_read(s32 a0, s32 a1, u8 *a2);
void printf();
void CdIntToPos(s32, u8 *);
void bcopy(u8 *src, u8 *dst, s32 len);

s32 CD_cachefile(s32 arg0) {
    u8 *buf;
    s32 i;
    u8 *base;
    u8 *dst;
    u8 *entryPtr;
    u8 *namePtr;
    s32 byteOff;
    s32 ret;
    s32 tmpVal;
    s32 tmpB;
    s32 tmpH;

    if (arg0 == libcd_cached_dir_id) {
        return 1;
    }

    buf = load_buf;
    ret = cd_read(1, libcd_dir_ent_lba_view[arg0].field_00, buf);

    if (ret != 1) {
        if (CD_debug > 0) {
            printf(libcd_cachefile_nodir_msg);
        }
        return -1;
    }

    do { i = 0; } while (0);

    if (CD_debug >= 2) {
        printf(libcd_cachefile_search_msg);
    }

    if (buf >= (load_buf + 2048)) {
        goto loop_done;
    }

    base = libcd_cached_file_tbl;
    namePtr = base + 8;

    do { entryPtr = base; } while (0);
    byteOff = 0;

    do {
    if (*buf == 0) goto loop_done;

    __builtin_memcpy(&tmpVal, buf + 2, 4);
    CdIntToPos(tmpVal, entryPtr);

    dst = base + 4;
    __builtin_memcpy((u8 *)(byteOff + (s32)dst), buf + 10, 4);

    switch (i) {
    case 0:
        *(u16 *)(base + 8) = libcd_dot_name_str;
        break;
    case 1:
        tmpH = libcd_dotdot_name_str;
        tmpB = libcd_dotdot_name_nul;
        tmpB = (s8)tmpB;
        *(s8 *)(base + 34) = tmpB;
        *(s16 *)(base + 32) = tmpH;
        break;
    default:
        bcopy(buf + 33, namePtr, buf[32]);
        namePtr[buf[32]] = 0;
        break;
    }

    entryPtr += 24;

    if (CD_debug >= 2) {
        printf(libcd_cachefile_entry_fmt,
                      libcd_cached_file_tbl[byteOff],
                      libcd_cached_file_tbl[byteOff + 1],
                      libcd_cached_file_tbl[byteOff + 2],
                      *(s32 *)&libcd_cached_file_tbl[byteOff + 4],
                      namePtr);
    }

    i++;
    buf += *buf;
    namePtr += 24;
    byteOff += 24;

    if (i >= 64) goto loop_done;
    } while (buf < (load_buf + 2048));

loop_done:
    libcd_cached_dir_id = arg0;

    if (i < 64) {
        libcd_cached_file_tbl[i * 24 + 8] = 0;
    }

    if (CD_debug >= 2) {
        printf(libcd_cachefile_count_fmt, i);
    }

    return 1;
}
