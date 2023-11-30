#ifndef _ENGINE_UI_H_
#define _ENGINE_UI_H_

#include "engine/item.h"

void ui_font_init(void);
void ui_timer_update(void);
void ui_prototype_draw(void);
void ui_item_qty_draw(const struct item *i, const u8 uses_2nd);

#endif /* _ENGINE_UI_H_ */
