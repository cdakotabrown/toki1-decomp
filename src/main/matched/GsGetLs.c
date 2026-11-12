/* Function: GsGetLs */
/* Address: 0x8009B170 */
/* Size: 584 bytes (146 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O2 --fill-earliest --jal-load-nop */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field 0x1c:rt:v1:a2 0x30:rt:a2:v1 0x38:rs:a2:v1 0x54:rd:v1:a2 0x64:rs:a2:v1 0x68:rd:v1:a2 0x80:rt:a2:v0 0x84:rs:a2:v0 0x84:rt:a2:v0 0x8c:rs:a2:v0 0x8c:rt:v0:v1 0x90:rs:a2:v0 0x90:rt:v1:a0 0x94:rs:a2:v0 0x94:rt:a0:a1 0x98:rs:a2:v0 0x98:rt:a1:a2 0x9c:rt:v0:v1 0xa0:rt:v1:a0 0xa4:rt:a0:a1 0xa8:rt:a1:a2 0xac:rs:a2:v0 0xac:rt:v0:v1 0xb0:rs:a2:v0 0xb0:rt:v1:a0 0xb4:rs:a2:v0 0xb4:rt:a0:a1 0xb8:rs:a2:v0 0xb8:rt:a1:a2 0xbc:rt:v0:v1 0xc0:rt:v1:a0 0xc4:rt:a0:a1 0xcc:rt:a1:a2 0xd0:rs:v1:a2 0xd8:rt:a2:v0 0xdc:rs:a2:v0 0xdc:rt:a2:v0 0xec:rs:v1:a2 0xf0:rt:v1:a2 0x100:rt:a2:v0 0x108:rs:a2:v0 0x108:rt:v0:v1 0x10c:rs:a2:v0 0x10c:rt:v1:a0 0x110:rs:a2:v0 0x110:rt:a0:a1 0x114:rs:a2:v0 0x114:rt:a1:a2 0x118:rt:v0:v1 0x11c:rt:v1:a0 0x120:rt:a0:a1 0x124:rt:a1:a2 0x128:rs:a2:v0 0x128:rt:v0:v1 0x12c:rs:a2:v0 0x12c:rt:v1:a0 0x130:rs:a2:v0 0x130:rt:a0:a1 0x134:rs:a2:v0 0x134:rt:a1:a2 0x138:rt:v0:v1 0x13c:rt:v1:a0 0x140:rt:a0:a1 0x148:rt:a1:a2 0x154:rt:a2:v0 0x158:rs:v1:a2 0x15c:rs:a2:v0 0x15c:rt:v0:v1 0x160:rs:a2:v0 0x160:rt:v1:a0 0x164:rs:a2:v0 0x164:rt:a0:a1 0x168:rs:a2:v0 0x168:rt:a1:a2 0x16c:rt:v0:v1 0x170:rt:v1:a0 0x174:rt:a0:a1 0x178:rt:a1:a2 0x17c:rs:a2:v0 0x17c:rt:v0:v1 0x180:rs:a2:v0 0x180:rt:v1:a0 0x184:rs:a2:v0 0x184:rt:a0:a1 0x188:rs:a2:v0 0x188:rt:a1:a2 0x18c:rt:v0:v1 0x190:rt:v1:a0 0x194:rt:a0:a1 0x198:rt:a1:a2 0x1a0:rd:v1:v0 0x1a4:rt:v0:v1 0x1a8:rs:v0:v1 0x1a8:rt:v0:v1 0x1ac:rs:v1:v0 0x1ac:rt:v0:v1 */
/* @PostProc: swap_instructions --offset1 0x8 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0xc --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x10 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x20 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x24 --offset2 0x28 */
/* @PostProc: swap_instructions --offset1 0xac --offset2 0xb8 */
/* @PostProc: swap_instructions --offset1 0xb0 --offset2 0xb4 */
/* @PostProc: swap_instructions --offset1 0xbc --offset2 0xcc */
/* @PostProc: swap_instructions --offset1 0xc0 --offset2 0xc4 */
/* @PostProc: swap_instructions --offset1 0x128 --offset2 0x134 */
/* @PostProc: swap_instructions --offset1 0x12c --offset2 0x130 */
/* @PostProc: swap_instructions --offset1 0x138 --offset2 0x148 */
/* @PostProc: swap_instructions --offset1 0x13c --offset2 0x140 */
/* @PostProc: swap_instructions --offset1 0x17c --offset2 0x188 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x18c --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x1c8 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x1cc --offset2 0x1d0 */
/* @PostProc: swap_instructions --offset1 0x1d0 --offset2 0x1d4 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x1d8 --offset2 0x1dc */
/* @PostProc: swap_instructions --offset1 0x1dc --offset2 0x1e0 */
/* @PostProc: swap_instructions --offset1 0x1e0 --offset2 0x1e4 */
/* @PostProc: swap_instructions --offset1 0x1e4 --offset2 0x1e8 */
/* @PostProc: swap_instructions --offset1 0x1e8 --offset2 0x1f8 */
/* @PostProc: swap_instructions --offset1 0x1ec --offset2 0x1f4 */
/* @PostProc: swap_instructions --offset1 0x1f8 --offset2 0x208 */
/* @PostProc: swap_instructions --offset1 0x1fc --offset2 0x204 */
/* @PostProc: swap_instructions --offset1 0x208 --offset2 0x20c */
/* (2) DELETED `@PostProc: scheduler_normalizer`.  It raised the masked exact% */
#include "types.h"

typedef struct {
    s32 f[4];
} Block4;

typedef struct {
    Block4 lo;
    Block4 hi;
} Block8;

typedef struct Node {
    s32 field0;
    Block8 data;
    Block8 result;
    s32 pad68;
    struct Node* next;
} NodeStruct;

extern void GsMulCoord3(s32*, s32*);
extern void GsMulCoord2(s32*, s32*);

extern NodeStruct* DSTACK[26];
extern s32 GsWSMATRIX;

void GsGetLs(NodeStruct* arg0, Block8* arg1) {
    register s32 count asm("s1");
    s32 index;
    NodeStruct* ptr;
    NodeStruct* next;
    NodeStruct* *ptrArray;

    ptrArray = &DSTACK[0];
    count = 0;
    index = 100;

loop:
    *ptrArray = arg0;
    next = arg0->next;
    if (next == 0) {
        if (arg0->field0 == 0) {
            index = count;
        }
        goto end_loop;
    }
    if (arg0->field0 == 0) {
        index = count;
    }
    arg0 = next;
    ptrArray++;
    count++;
    goto loop;

end_loop:
    if (count == 0) {
        ptr = DSTACK[0];
        *arg1 = ptr->data;
        goto after_copy;
    }

    if (index == 100) {
        ptr = DSTACK[0];
        count = 0;
        goto copy_result;
    }

    if ((index ^ count) != 0) {
        ptr = DSTACK[index + 1];
        count = index + 1;
        goto copy_result;
    }

    ptr = DSTACK[count];
    *arg1 = ptr->data;
    goto after_copy;

copy_result:
    *arg1 = ptr->result;

after_copy:
    if (count > 0) {
        s32 i = count;
        NodeStruct* *pEntry = &DSTACK[count - 1];
        do {
            GsMulCoord3((s32*)arg1, (s32*)&(*pEntry)->data);
            (*pEntry)->result = *arg1;
            (*pEntry)->field0 = 1;
            i--;
            pEntry--;
        } while (i > 0);
    }

tail:
    GsMulCoord2((s32*)&GsWSMATRIX, (s32*)arg1);
}
