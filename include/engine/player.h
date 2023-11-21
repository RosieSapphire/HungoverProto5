#ifndef _ENGINE_PLAYER_H_
#define _ENGINE_PLAYER_H_

#include "engine/types.h"
#include "engine/input.h"
#include "engine/camera.h"
#include "engine/item.h"

enum item_selected
{
	ITEM_SELECT_NONE = -1,
	ITEM_SELECT_PISTOL,
	ITEM_SELECT_BONG,
};

enum item_equipped_flags
{
	ITEM_HAS_NONE   = 0x0,
	ITEM_HAS_PISTOL = 0x1,
	ITEM_HAS_BONG   = 0x2,
};

struct player
{
	struct camera view;
	f32 pos[3], vel[3];
	u8 item_flags;
	u8 item_selected;
	struct item pistol;
	f32 recoil_amnt, recoil_amnt_last;
	f32 recoil_dir[2];
};

void player_init(struct player *p, u8 items_equipped_flags);
void player_update(struct player *p, const struct input_parms iparms);
void player_items_update(struct player *p, const struct input_parms iparms);
void player_item_draw(const struct player *p, const f32 subtick);
void player_camera_view_matrix_setup(const struct player *p,
				     const f32 subtick);

#endif /* _ENGINE_PLAYER_H_ */
