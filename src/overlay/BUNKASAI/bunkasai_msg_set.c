/* Address: 0x80147F8C */
/* Size: 176 bytes (44 insns) */
/* @SymbolSize: 176 */
/* @PostProc: patch_stack_complete --old 328 --new 344 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

typedef struct {
    char x[297];
} MsgTable;

extern MsgTable bunkasai_msg_set_tbl;
extern u8  now_year;
extern s32 serifu_no;
extern char aname[];

extern void strcpy(char *, char *);

void bunkasai_msg_set(void) {
    MsgTable buf;

    buf = bunkasai_msg_set_tbl;
    strcpy(aname,
                  &buf.x[(now_year - 95) * 99 + serifu_no * 33]);
}
