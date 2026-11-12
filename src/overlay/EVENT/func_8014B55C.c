/* @RodataBase: 0x8011FB80 */
/* @PostProc: patch_stack_complete --old 288 --new 312 */
/* @PostProc: remap_stack_slots --from 44 --to 48 */
/* @PostProc: patch_insn_reg_field 0x14:rt:a0:a1 0x1c:rt:a1:a0 0x24:rs:a0:a1 0x24:rt:a0:a1 0x30:rs:a1:a0 0x5c:rt:a0:a1 0x60:rd:v1:v0 0x60:rs:a1:a0 0x68:rs:v1:v0 0x68:rt:v0:v1 0x74:rs:v0:t2 0x74:rt:t2:v1 0x78:rt:a0:t3 0x7c:rs:a0:t3 0x7c:rt:a0:t3 0x84:rs:a0:t3 0x84:rt:a0:t4 0x88:rs:a0:t4 0x90:rd:t3:t4 0x90:rt:a0:t4 0x98:rt:t3:t4 0x9c:rt:t3:t4 0xa4:rs:t3:t4 0xac:rt:a0:t5 0xb0:rs:a0:t5 0xb0:rt:v0:v1 0xb8:rs:v1:v0 0xb8:rt:v0:v1 0xbc:rt:a0:t8 0xc0:rs:a0:t8 0xc0:rt:v0:v1 0xc8:rs:v1:v0 0xc8:rt:v0:v1 0xcc:rt:a0:t7 0xd0:rs:a0:t7 0xd0:rt:v0:v1 0xd8:rs:v1:v0 0xd8:rt:v0:v1 0xdc:rt:a0:t9 0xe0:rs:a0:t9 0xe0:rt:v0:v1 0xe8:rs:v1:v0 0xe8:rt:v0:v1 0xec:rt:a0:t6 0xf0:rs:a0:t6 0xf0:rt:v0:v1 0xf8:rs:v1:v0 0xf8:rt:v0:v1 0xfc:rt:a0:t0 0x100:rs:a0:t0 0x100:rt:v0:v1 0x108:rs:v1:v0 0x108:rt:v0:v1 0x10c:rt:a0:t1 0x110:rs:a0:t1 0x110:rt:v0:v1 0x118:rs:v1:v0 0x118:rt:v0:v1 0x11c:rt:a0:t2 0x120:rs:a0:t2 0x120:rt:v0:v1 0x128:rs:v1:v0 0x128:rt:v0:v1 0x12c:rt:a0:t3 0x130:rs:a0:t3 0x130:rt:v0:v1 0x138:rs:v1:v0 0x138:rt:v0:v1 0x13c:rt:a0:t4 0x140:rs:a0:t4 0x140:rt:v0:v1 0x148:rs:v1:v0 0x148:rt:v0:v1 0x14c:rt:a0:t5 0x150:rs:a0:t5 0x150:rt:v0:v1 0x158:rs:v1:v0 0x158:rt:v0:v1 0x15c:rt:a0:t8 0x160:rs:a0:t8 0x160:rt:v0:v1 0x168:rs:v1:v0 0x168:rt:v0:v1 0x16c:rt:a0:t7 0x170:rs:a0:t7 0x170:rt:v0:v1 0x178:rs:v1:v0 0x178:rt:v0:v1 0x17c:rt:a0:t9 0x180:rs:a0:t9 0x180:rt:v0:v1 0x188:rs:v1:v0 0x188:rt:v0:v1 0x18c:rt:a0:t6 0x190:rs:a0:t6 0x190:rt:v0:v1 0x198:rs:v1:v0 0x198:rt:v0:v1 0x19c:rt:a0:t0 0x1a0:rs:a0:t0 0x1a0:rt:v0:v1 0x1a8:rs:v1:v0 0x1a8:rt:v0:v1 0x1ac:rt:a0:t1 0x1b0:rs:a0:t1 0x1b0:rt:v0:v1 0x1b8:rs:v1:v0 0x1b8:rt:v0:v1 0x1bc:rt:a0:t2 0x1c0:rs:a0:t2 0x1c0:rt:v0:v1 0x1c8:rs:v1:v0 0x1c8:rt:v0:v1 0x1cc:rt:a0:t3 0x1d0:rs:a0:t3 0x1d0:rt:v0:v1 0x1d8:rs:v1:v0 0x1d8:rt:v0:v1 0x1dc:rt:a0:t4 0x1e0:rs:a0:t4 0x1e0:rt:v0:v1 0x1e4:rs:v1:v0 0x1e4:rt:v0:v1 0x1e8:rs:v0:v1 */
/* Address: 0x8014B55C */
/* Size: 496 bytes (124 instructions) */
#include "types.h"

typedef void (*func_ptr_t)(s32);

typedef struct {
    u32 v[61];
} Table;

extern Table D_801243B8;

extern volatile u8 D_800B1AF6;

extern volatile u8 D_801243B0;

extern void func_80111E60(s32);
extern void func_80111F04(s32);
extern void func_801139CC(s32);
extern void func_8011220C(s32);
extern void func_80112460(s32);
extern void func_80112750(s32);
extern void func_80112930(s32);
extern void func_80112BEC(s32);
extern void func_801130F0(s32);
extern void func_80113300(s32);
extern void func_80113520(s32);
extern void func_801136FC(s32);
extern void func_801138D8(s32);
extern void func_80113A40(s32);
extern void func_80113CA0(s32);
extern void func_80113FD4(s32);
extern void func_801142E8(s32);
extern void func_801144AC(s32);
extern void func_801146B4(s32);
extern void func_80114784(s32);
extern void func_80114948(s32);

void func_8014B55C(void) {
    Table table;
    u8 charId;
    s32 sceneIdx;
    u32 *entryPtr;
    func_ptr_t funcPtr;

    charId = D_800B1AF6;
    table = D_801243B8;

    entryPtr = &table.v[charId];
    funcPtr = (func_ptr_t)*entryPtr;

    if (funcPtr == func_80111E60) {
        sceneIdx = D_801243B0 - 1;

        {
            switch (sceneIdx) {
                case 0:  funcPtr = func_80111F04; *entryPtr = (u32)funcPtr; break;
                case 1:  funcPtr = func_801139CC; *entryPtr = (u32)funcPtr; break;
                case 2:  funcPtr = func_8011220C; *entryPtr = (u32)funcPtr; break;
                case 3:  funcPtr = func_80112460; *entryPtr = (u32)funcPtr; break;
                case 4:  funcPtr = func_80112750; *entryPtr = (u32)funcPtr; break;
                case 5:  funcPtr = func_80112930; *entryPtr = (u32)funcPtr; break;
                case 6:  funcPtr = func_80112BEC; *entryPtr = (u32)funcPtr; break;
                case 7:  funcPtr = func_801130F0; *entryPtr = (u32)funcPtr; break;
                case 8:  funcPtr = func_80113300; *entryPtr = (u32)funcPtr; break;
                case 9:  funcPtr = func_80113520; *entryPtr = (u32)funcPtr; break;
                case 10: funcPtr = func_801136FC; *entryPtr = (u32)funcPtr; break;
                case 11: funcPtr = func_801138D8; *entryPtr = (u32)funcPtr; break;
                case 12: funcPtr = func_80113A40; *entryPtr = (u32)funcPtr; break;
                case 13: funcPtr = func_80113CA0; *entryPtr = (u32)funcPtr; break;
                case 14: funcPtr = func_80113FD4; *entryPtr = (u32)funcPtr; break;
                case 15: funcPtr = func_801142E8; *entryPtr = (u32)funcPtr; break;
                case 16: funcPtr = func_801144AC; *entryPtr = (u32)funcPtr; break;
                case 17: funcPtr = func_801146B4; *entryPtr = (u32)funcPtr; break;
                case 18: funcPtr = func_80114784; *entryPtr = (u32)funcPtr; break;
                case 19: funcPtr = func_80114948; *entryPtr = (u32)funcPtr; break;
            }
        }
    }

    funcPtr(128);
}
