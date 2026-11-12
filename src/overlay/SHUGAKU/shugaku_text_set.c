/* @PostProc: remap_stack_slots --map '28:40' */
/* @PostProc: patch_stack_complete --old 3296 --new 3312 */
#include "types.h"

typedef struct {
    u8 b[3267];
} Blk;

extern Blk  shugaku_text_set_tbl;
extern u8   now_girl;
extern u8   shugaku_gakunen;
extern s32  serifu_no;
extern u8   aname;
extern void strcpy(u8 *a0, u8 *a1);

void shugaku_text_set(void)
{
    Blk buf;

    buf = shugaku_text_set_tbl;
    strcpy(&aname,
                  &buf.b[now_girl * 297 + shugaku_gakunen * 99 +
                         serifu_no * 33]);
}
