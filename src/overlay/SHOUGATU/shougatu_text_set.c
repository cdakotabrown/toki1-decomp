/* @CompileFlags: -Wc,-pic0 */
/* Address: 0x80136864 */
/* Size: 196 bytes (49 instructions) */
#include "types.h"

typedef struct {
    u8 b[3267];
} Blob;

extern void strcpy(void *dst, void *src);
extern Blob shougatu_text_set_tbl;
extern u8 now_girl;
extern u8 now_year;
extern s32 serifu_no;
extern u8 aname;

void shougatu_text_set(void)
{
    u8 above[4];
    Blob buf;
    u8 below[12];

    (void)&above;
    (void)&below;

    buf = shougatu_text_set_tbl;
    strcpy(&aname,
                  &((u8 *)&buf)[now_girl * 297 + now_year * 99 +
                                serifu_no * 33 - 9504]);
}
