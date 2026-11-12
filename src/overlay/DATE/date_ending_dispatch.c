/* @SymbolStart: 0x80146194 */
/* @SymbolSize: 140 */
/* @RodataBase: 0x8015B1A4 */
/* Function: date_ending_dispatch */
/* Overlay: DATE */
/* Address: 0x80146194 (cross-prologue head) / 0x8014619C (body) */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

extern u8 date_ride_kind;

extern void date_intro_script(void);
extern void date_ending_script2(void);
extern void date_ending_script0(void);
extern void date_ending_script1(void);
extern void date_sub_dispatch(void);

void date_ending_dispatch(void) {
    switch (date_ride_kind) {
        case 0: date_ending_script0(); break;
        case 1: date_ending_script1(); break;
        case 2: date_ending_script2(); break;
        case 3: date_intro_script(); break;
        case 4: date_sub_dispatch(); break;
    }
}
