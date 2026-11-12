/* Address: 0x80053BE0 */
/* Size: 72 bytes */
/* Region: GCC (0x80040000-0x80080000) */
/* Compiler: IDO 5.2 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void Sw_Start(void);
extern void Hw_Start(void);
extern void InitCARD(s32 a0);
extern void StartCARD(void);
extern void _bu_init(void);
extern void _card_auto(s32 a0);

void card_ev_set(void) {
    Sw_Start();
    Hw_Start();
    InitCARD(1);
    StartCARD();
    _bu_init();
    _card_auto(0);
}
