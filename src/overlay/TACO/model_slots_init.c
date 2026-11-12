/* @SymbolSize: 124 */
/* Address: 0x801321A0 */
/* Size: 124 bytes (31 insns) */
#include "types.h"

extern void model_slot_set();

void model_slots_init(void) {
    model_slot_set((void*)0x8018A000, 0, 0);
    model_slot_set((void*)0x8018A000, 1, 0);
    model_slot_set((void*)0x8018A000, 2, 0);
    model_slot_set((void*)0x8018A000, 3, 0);
    model_slot_set((void*)0x8018A000, 4, 0);
}
