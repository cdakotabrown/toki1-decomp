/* kangei_text_set - KANGEI overlay, Address: 0x80139548, 192 bytes. */
#include "types.h"

typedef struct {
    char s[11][3][3][33];
} StrTbl;

extern void strcpy(void *, void *);

extern StrTbl kangei_text_set_tbl;
extern u8 now_girl;
extern u8 kangei_year_no;
extern s32 serifu_no;
extern u8 aname[];

void kangei_text_set(void)
{
    s32 pad;
    StrTbl tbl;

    tbl = kangei_text_set_tbl;
    strcpy(aname, tbl.s[now_girl][kangei_year_no][serifu_no]);
}
