/* @PostProc: patch_stack_complete --old 3296 --new 3312 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
/* Address: 0x80137A2C */
/* Size: 196 bytes (49 insns) */
#include "types.h"

typedef struct {
    u8 b[3267];
} Blk;

extern Blk date2_msg_set_tbl;
extern u8 aname[];
extern u8 now_girl;
extern u8 now_year;
extern s32 serifu_no;

extern void strcpy(void *dst, void *src);

void date2_msg_set(void)
{
    Blk buf;

    buf = date2_msg_set_tbl;
    strcpy(aname,
                  &buf.b[297 * now_girl + 99 * now_year
                         + 33 * serifu_no - 9405]);
}
