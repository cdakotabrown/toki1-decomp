/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x80137848 */
/* Size: 192 bytes (48 insns) */
#include "types.h"

typedef struct {
    u8 d[11][3][3][33];
} NameTable;

extern NameTable geko_name_set_tbl;
extern u8 now_girl;
extern u8 geko_gakunen;
extern s32 serifu_no;
extern u8 aname[];

extern void strcpy(u8 *, u8 *);

void geko_name_set(void)
{
    s32 pre[1];
    NameTable buf;
    s32 pad[3];

    (void)&pre;
    (void)&pad;
    buf = geko_name_set_tbl;
    strcpy(aname,
                  buf.d[now_girl][geko_gakunen][serifu_no]);
}
