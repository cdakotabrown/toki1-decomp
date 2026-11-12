/* Function: CdSearchFile  --  Sony libcd `CdSearchFile` */
/* Address: 0x80088F90 */
/* Size: 724 bytes (181 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 -fsigned-char -mhard-float --fill-epilogue --fill-branch-store --jal-load-nop --fill-j --load-dest-temp --fill-ptr-bump-down --label-load-nop */
/* @PostProc: patch_insn_reg_field 0x88:rt:s5:s4 0x90:rt:s4:s5 0xa0:rt:s4:s5 0xf0:rt:s5:s4 */
/* @PostProc: swap_instructions --offset1 0x0 --offset2 0x10 */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x2c */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x2c --offset2 0x30 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x25c --offset2 0x260 */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x274 */
/* @PostProc: swap_instructions --offset1 0x270 --offset2 0x278 */
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;

typedef struct {
    CdlLOC pos;
    u32 size;
    char name[16];
} CdlFILE;

extern s32 libcd_media_gen_seen;
extern s32 CD_debug;
extern s32 CD_status1;
extern CdlFILE libcd_cached_file_tbl[64];

extern s32 CD_newmedia();
extern s32 CD_searchdir();
extern s32 CD_cachefile();
extern s32 _cmp();
extern void printf();

CdlFILE *CdSearchFile(CdlFILE *fp, char *name)
{
    char fname[32];
    char *p;
    char *q;
    s32 i;
    s32 dir;
    s32 nf;

    if (libcd_media_gen_seen != CD_status1) {
        while (CD_newmedia() == 0)
            ;
        libcd_media_gen_seen = CD_status1;
    }

    dir = 1;
    if (*name != '\\')
        return 0;

    fname[0] = 0;
    p = name;
    nf = -1;
    for (i = 0; i < 8; i++) {
        q = fname;
        while (*p != '\\' && *p != 0) {
            *q++ = *p;
            p++;
        }
        if (*p == 0)
            break;
        p++;
        *q = 0;
        dir = CD_searchdir(dir, fname);
        if (dir == nf) {
            fname[0] = 0;
            break;
        }
    }

    if (i >= 8) {
        if (CD_debug > 0)
            printf("%s: path level (%d) error\n", name, i);
        return 0;
    }

    if (fname[0] == 0) {
        if (CD_debug > 0)
            printf("%s: dir was not found\n", name);
        return 0;
    }

    *q = 0;
    if (CD_cachefile(dir) == 0) {
        if (CD_debug > 0)
            printf("CdSearchFile: disc error\n");
        return 0;
    }

    if (CD_debug >= 2)
        printf("CdSearchFile: searching %s...\n", fname);

    for (i = 0; i < 64; i++) {
        if (libcd_cached_file_tbl[i].name[0] == 0)
            break;
        if (_cmp(libcd_cached_file_tbl[i].name, fname)) {
            if (CD_debug >= 2)
                printf("%s:  found\n", fname);
            *fp = libcd_cached_file_tbl[i];
            return &libcd_cached_file_tbl[i];
        }
    }

    if (CD_debug > 0)
        printf("%s: not found\n", fname);
    return 0;
}
