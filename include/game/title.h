#ifndef _GAME_TITLE_H_
#define _GAME_TITLE_H_

#include "engine/input.h"
#include "engine/types.h"

void title_load(void);
void title_unload(void);
enum scene_index title_update(struct input_parms iparms);
void title_draw(f32 subtick);

#endif /* _GAME_TITLE_H_ */
