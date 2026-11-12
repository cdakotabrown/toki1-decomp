/* Address: 0x80134894 */
/* Size: 184 bytes (46 insns) */
/* @SymbolSize: 168 */
#include "types.h"

extern u8 taco_effect_buf;
extern u8 taco_effect2_buf;
extern u8 taco_actor_buf;
extern u8 taco_actor2_buf;
extern u8 taco_throw_tbl;
extern u8 taco_spark_buf;
extern u8 taco_spark2_buf;
extern u8 taco_pickup_tbl;
extern u8 taco_item_pool;

extern void bzero();

void taco_work_clear(void)
{
    bzero(&taco_effect_buf, 320);
    bzero(&taco_effect2_buf, 320);
    bzero(&taco_actor_buf, 320);
    bzero(&taco_actor2_buf, 320);
    bzero(&taco_throw_tbl, 320);
    bzero(&taco_spark_buf, 320);
    bzero(&taco_spark2_buf, 320);
    bzero(&taco_pickup_tbl, 320);
    bzero(&taco_item_pool, 320);
}
