#ifndef _ENGINE_PLAYER_H_
#define _ENGINE_PLAYER_H_

#include "engine/types.h"
#include "engine/input.h"
#include "engine/camera.h"
#include "engine/item.h"
#include "engine/collision_mesh.h"

enum item_selected
{
	ITEM_SELECT_NONE = -1,
	ITEM_SELECT_PISTOL,
	ITEM_SELECT_BONG,
	ITEM_COUNT,
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
	f32 pos[3], vel[3], vel_last[3];
	u8 item_flags;
	s8 item_selected;
	struct item items[ITEM_COUNT];
	f32 recoil_amnt, recoil_amnt_last;
	f32 recoil_dir[2];
	f32 turn_offset[2], turn_offset_last[2];
	f32 headbob_timer, headbob_timer_last;
	f32 weed_high_amnt;
	u16 weed_progress, weed_duration;
};

/*
 * Main
 */
void player_init(const struct scene *s, struct player *p,
		 u8 items_equipped_flags);
void player_update(struct scene *s, struct player *p,
		   const struct input_parms iparms);

/*
 * Camera
 */
void player_camera_view_matrix_setup(const struct player *p,
				     const f32 subtick);
void player_camera_look_update(struct player *p,
			       const struct input_parms iparms);

/*
 * Physics
 */
void player_friction(struct player *p);
void player_accelerate(struct player *p, const struct input_parms iparms);
void player_collision(const struct collision_mesh *m, struct player *p,
		      enum collision_type coltype);

/*
 * Items
 */
void player_item_load(struct player *p, const u8 flags_last);
void player_check_pickup(struct scene *s, struct player *p);
void player_items_update(struct player *p, const struct input_parms iparms);
void player_item_draw(const struct player *p, const f32 subtick);

#endif /* _ENGINE_PLAYER_H_ */
