/* Address: 0x80089284 */
/* Size: 712 bytes (178 instructions) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -G0 -mips1 -mhard-float -fno-schedule-insns --fill-epilogue --load-dest-temp --fill-load-delay --jal-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x174 --offset2 0x178 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x17c */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x19c */
/* @PostProc: swap_instructions --offset1 0x19c --offset2 0x1a0 */
/* @PostProc: swap_instructions --offset1 0x1a0 --offset2 0x1a4 */
/* @PostProc: swap_instructions --offset1 0x1a4 --offset2 0x1a8 */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1ac */
/* @PostProc: swap_instructions --offset1 0x1ac --offset2 0x1b0 */
/* @PostProc: swap_instructions --offset1 0x200 --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x204 --offset2 0x214 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x210 --offset2 0x218 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x21c --offset2 0x220 */
#include "types.h"

typedef struct {
    s32 id;
    s32 parent;
    s32 lba;
    char name[32];
} CdDirEnt;

extern s32 CD_debug;
extern u8  load_buf[2048];
extern CdDirEnt dire[128];
extern s32 libcd_cached_dir_id;
extern char libcd_newmedia_pvd_err_fmt[];
extern char libcd_iso_id_str[];
extern char libcd_newmedia_format_err_msg[];
extern char libcd_newmedia_pt_err_fmt[];
extern char libcd_newmedia_search_msg[];
extern char libcd_newmedia_dirent_fmt[];
extern char libcd_newmedia_count_fmt[];

s32 cd_read(s32 a0, s32 a1, u8 *a2);
s32 strncmp(u8 *a0, char *a1, s32 a2);
void bcopy(u8 *src, char *dst, s32 len);
void printf(char *fmt, ...);
void *memcpy(void *dst, const void *src, unsigned int n);

s32 CD_newmedia(void) {
    u8 *cp;
    u8 *end;
    s32 n;
    s32 result;
    s32 ptblk;
    s32 pid;

    result = cd_read(1, 16, load_buf);
    if (result != 1) {
        if (CD_debug > 0) {
            printf(libcd_newmedia_pvd_err_fmt);
        }
        return 0;
    }
    if (strncmp(load_buf + 1, libcd_iso_id_str, 5) != 0) {
        if (CD_debug > 0) {
            printf(libcd_newmedia_format_err_msg);
        }
        return 0;
    }
    memcpy(&ptblk, load_buf + 140, 4);
    if (cd_read(1, ptblk, load_buf) != result) {
        if (CD_debug > 0) {
            printf(libcd_newmedia_pt_err_fmt, ptblk);
        }
        return 0;
    }
    cp = load_buf;
    if (CD_debug >= 2) {
        printf(libcd_newmedia_search_msg);
    }

    n = 0;
    while (cp < load_buf + 2048 && *cp != 0) {
        memcpy(&dire[n].lba, cp + 2, 4);
        pid = cp[6];
        dire[n].id = n + 1;
        dire[n].parent = pid;
        bcopy(cp + 8, dire[n].name, *cp);
        dire[n].name[*cp] = 0;
        cp += *cp + 8 + (*cp % 2);
        if (CD_debug >= 2) {
            printf(libcd_newmedia_dirent_fmt, dire[n].lba, dire[n].id,
                          dire[n].parent, dire[n].name);
        }
        n++;
        if (n >= 128) {
            goto done;
        }
    }
    if (n < 128) {
        dire[n].parent = 0;
    }
done:
    libcd_cached_dir_id = 0;
    if (CD_debug >= 2) {
        printf(libcd_newmedia_count_fmt, n);
    }
    return 1;
}
