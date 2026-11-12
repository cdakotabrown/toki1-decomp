/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=0 IDO52_PRECOLOR_WEB=v0:6 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* func_8013C638 -- GYOZI overlay, Address: 0x8013C638 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 236 */
#include "types.h"

typedef struct {
    u8 pad[56];
} Rec;

extern Rec D_800F53A0[];
extern volatile u8 D_800F62CF;

extern void func_8008FB00(void);
extern void func_80050D60(s32, s32);
extern void func_80081D30(s32);

void func_8013C638(void)
{
    s16 i;
    u8 *e;

    i = 0;
    do {
        e = (u8 *)&D_800F53A0[i];
        if ((e[441] & 1) != 0) {
            if (i != D_800F62CF) {
                *(s16 *)&e[434] += 1;
            }
        }
        i = i + 1;
    } while (i < 11);

    e = (u8 *)&D_800F53A0[D_800F62CF];
    *(s16 *)&e[430] += 8;
    e = (u8 *)&D_800F53A0[D_800F62CF];
    *(s16 *)&e[434] += 8;

    func_8008FB00();
    func_80050D60(0, 0);
    func_80081D30(1);
}
