/* Function: mem2mem */
/* Address: 0x80088C44 */
/* Size: 48 bytes (12 instructions) */
/* Region: IDO (Library, >= 0x80086000) */
/* Likely Symbol: mem2mem (from o.bin) */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_registers at v0 */
typedef unsigned int u32;

void mem2mem(u32 *arg0, u32 *arg1, u32 arg2)
{
    u32 var_v1;
    u32 temp_v0;

    if (!temp_v0)
    {
    }

    var_v1 = 0;
    if (arg2 != 0)
    {
        do
        {
            temp_v0 = *arg1;
            var_v1 += 1;
            *arg0 = temp_v0;

            if (((!var_v1) && (!var_v1)) && (!var_v1))
            {
            }

            arg1++;
            arg0++;
        }
        while (var_v1 < arg2);
    }
}
