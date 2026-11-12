/* Address: 0x8004264C - 0x8004267C (48 bytes) */
/* Region: GCC (< 0x80081000) */
/* Compiler: IDO 5.2 (via static-recomp) */
typedef signed int s32;

extern void EnterCriticalSection(void);
extern void FlushCache(void);
extern void ExitCriticalSection(void);

void cache_flush_critical(void) {
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}
