/* Address: 0x8004DE24 */
/* Size: 224 bytes (56 instructions) -- the old 36-byte window ended in the */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u32 n0 : 4;
    u32 n1 : 4;
    u32 n2 : 4;
    u32 n3 : 4;
    u32 n4 : 4;
    u32 n5 : 4;
    u32 n6 : 4;
    u32 n7 : 4;
} Nibbles;

extern char printpix_str0[];
extern char printpix_str1[];
extern char printpix_str2[];
extern char printpix_str3[];
extern char printpix_str4[];
extern char printpix_str5[];
extern char printpix_str6[];
extern char printpix_str7[];
extern void printf(char *, s32);

void printpix(Nibbles *p)
{
    printf(printpix_str0, p->n3);
    printf(printpix_str1, p->n2);
    printf(printpix_str2, p->n1);
    printf(printpix_str3, p->n0);
    printf(printpix_str4, p->n7);
    printf(printpix_str5, p->n6);
    printf(printpix_str6, p->n5);
    printf(printpix_str7, p->n4);
}
