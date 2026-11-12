/* Function: startIntr */
/* Address: 0x800B3BF4 */
/* Size: 188 bytes (47 instructions) */
/* Region: Library (0x80086000+) */
/* @Compiler: gcc260 */
/* @CompileFlags: -O1 --delay-slot-load-nop --konami-lib-sched */
/* @PostProc: maspsx --aspsx-version=2.67 */
/* @PostProc: fill_gcc_delay_slots --fill-epilogue */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    u16 flag;
    char pad[50];
    void *ptr;
} GsStruct;

extern GsStruct libapi_intr_env;
extern void *libetc_intr_vtbl_ptr;
extern void *libapi_intr_jmpbuf_sp;
extern u8 libapi_intr_stack_buf[];

extern void EnterCriticalSection(void);
extern void memclr(GsStruct *arg0, s32 arg1);
extern s32 setjmp(void *arg0);
extern void trapIntr(void);
extern s32 startIntrVSync(void);
extern s32 startIntrDMA(void);
extern void _96_remove(void *arg0);
extern void HookEntryInt(void *arg0);
extern void ExitCriticalSection(void);

typedef struct {
    s32 field_00;
    void *field_04;
    char pad[12];
    void *field_14;
} GsStruct2;

void startIntr(void) {
    GsStruct *s0;
    GsStruct2 *v1;
    GsStruct2 *a0;
    s32 ret;

    s0 = &libapi_intr_env;

    if (s0->flag != 0) {
        return;
    }

    EnterCriticalSection();
    memclr(s0, 25);

    ret = setjmp(&s0->ptr);
    if (ret != 0) {
        trapIntr();
    }

    s0 = &libapi_intr_env;
    s0->flag = 1;

    ret = startIntrVSync();
    v1 = (GsStruct2 *)libetc_intr_vtbl_ptr;
    v1->field_14 = (void *)ret;

    ret = startIntrDMA();
    a0 = (GsStruct2 *)libetc_intr_vtbl_ptr;
    a0->field_04 = (void *)ret;
    _96_remove(a0);

    libapi_intr_jmpbuf_sp = (void *)libapi_intr_stack_buf;
    HookEntryInt(&s0->ptr);
    ExitCriticalSection();
}
