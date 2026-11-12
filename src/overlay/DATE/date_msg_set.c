/* Size: 192 bytes */
/* @SymbolSize: 192 */
/* @PostProc: remap_stack_slots --from 44 --to 40 */
#include "types.h"

typedef struct { u8 x[33]; } Entry;
typedef struct { Entry e[11][3][3]; } Blob;

extern void strcpy(void *a0, void *a1);

extern Blob date_msg_set_tbl;
extern u8 aname[];
extern u8 now_girl;
extern u8 date_gakunen;
extern s32 serifu_no;

void date_msg_set(void) {
    Blob buf;

    buf = date_msg_set_tbl;
    strcpy(aname,
                  &buf.e[now_girl][date_gakunen][serifu_no]);
}
