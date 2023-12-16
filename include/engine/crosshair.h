/**
 * @file include/engine/crosshair.h
 */

#ifndef _ENGINE_CROSSHAIR_H_
#define _ENGINE_CROSSHAIR_H_

#include <libdragon.h>

#include "engine/types.h"

/**
 * @defgroup engine Engine Functions
 * @{
 * @defgroup crosshair Crosshair Functions
 * @{
 */
rspq_block_t *crosshair_rspq_block_gen(const u16 dim);
void crosshair_draw(rspq_block_t *crosshair_block);
/**
 * @}
 * @}
 */

#endif /* _ENGINE_CROSSHAIR_H_ */
