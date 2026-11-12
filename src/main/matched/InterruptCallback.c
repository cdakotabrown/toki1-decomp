/* Function: InterruptCallback */
/* Address: 0x800B3AF0 */
/* Size: 48 bytes (12 instructions) */
/* Likely Symbol: ResetCallback (vtable series) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @PostProc: strip_leading_instructions --count 19 */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots */
#include "types.h"

typedef void (*func_ptr)(void);

typedef struct {
    func_ptr f0;
    func_ptr f4;
    func_ptr f8;
    func_ptr f12;
} vtable_t;

extern vtable_t *libetc_intr_vtbl_ptr;

void InterruptCallback(void) {
    libetc_intr_vtbl_ptr->f8();
}
