typedef signed int s32;
extern void func_801ED9F0(s32);

/* Test with unused padding */
void test_with_pad(s32 arg0) {
    s32 _pad[4];
    unsigned short new_var;
    func_801ED9F0(new_var = arg0 & 0xFFFF);
}

/* Test baseline */
void test_baseline(s32 arg0) {
    unsigned short new_var;
    func_801ED9F0(new_var = arg0 & 0xFFFF);
}
