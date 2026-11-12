/* Address: 0x800A0130 */
/* Size: 80 bytes (20 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: convert_or_to_addu */
/* @PostProc: patch_stack_complete --old 24 --new 32 --shift-ra */
/* @PostProc: swap_registers t6:v0 --only 0x20,0x24,0x2c,0x34 */
/* @PostProc: swap_registers t9:t6 --only 0x2c,0x38 */
typedef void (*FuncPtr)(int, int, int, int);

typedef struct {
    int pad[2];
    FuncPtr func;
    int pad2[4];
    int value;
} SomeStruct;

extern void checkRECT(int addr, int arg);
extern SomeStruct *graph_jtb;
extern unsigned char libgpu_storeimage_name_str[];

void StoreImage(int arg0, int arg1) {
    checkRECT((int)libgpu_storeimage_name_str, arg0);
    graph_jtb->func(graph_jtb->value, arg0, 8, arg1);
}
