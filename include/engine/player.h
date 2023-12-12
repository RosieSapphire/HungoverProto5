#ifndef _ENGINE_PLAYER_H_
#define _ENGINE_PLAYER_H_

#include <libdragon.h>

#include "engine/types.h"
#include "engine/input.h"
#include "engine/camera.h"
#include "engine/item.h"
#include "engine/collision_mesh.h"

/**
 * enum item_selected - Item Currently Selected
 * @ITEM_SELECT_NONE: No Item
 * @ITEM_SELECT_PISTOL: Pistol Selected
 * @ITEM_SELECT_BONG: Bong Selected
 * @ITEM_SELECT_NITROUS: Nitrous Canister Selected
 * @ITEM_COUNT: Max Number of Items
 */
enum item_selected
{
	ITEM_SELECT_NONE = -1,
	ITEM_SELECT_PISTOL,
	ITEM_SELECT_BONG,
	ITEM_SELECT_NITROUS,
	ITEM_COUNT,
};

/**
 * enum item_equipped_flags - Flags for Which Items Player Has
 * @ITEM_HAS_NONE: Empty Flag
 * @ITEM_HAS_PISTOL: Pistol Flag
 * @ITEM_HAS_BONG: Bong Flag
 * @ITEM_HAS_NITROUS: Nitrous Flag
 */
enum item_equipped_flags
{
	ITEM_HAS_NONE    = 0x0,
	ITEM_HAS_PISTOL  = 0x1,
	ITEM_HAS_BONG    = 0x2,
	ITEM_HAS_NITROUS = 0x4,
};

/**
 * struct player - Player Structure
 * @view: Camera for View Matrix
 * @pos: Position
 * @vel: Velocity
 * @vel_last: Previous Velocity
 * @item_flags: Which Items Player Has
 * @item_selected: Currently Selected Item
 * @items: Items Array
 * @recoil_amnt: Current Amount of Recoil
 * @recoil_amnt_last: Previous Amount of Recoil
 * @recoil_dir: Recoil Direction
 * @turn_offset: Current Turn Offset (Coughing, Recoil, etc.)
 * @turn_offset_last: Previous Turn Offset
 * @headbob_timer: Current Headbob Timer
 * @headbob_timer_last: Previous Headbob Timer
 * @weed_high_amnt: Weed High Amount
 * @weed_progress: Weed Progress
 * @weed_duration: Used as a percentage for `weed_progress`
 */
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
void player_terminate(struct player *p);
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

/*
 * Collision
 */
void player_collision(const struct collision_mesh *m, struct player *p,
		      enum collision_type coltype);

/*
 * Items
 */
void player_item_load(struct player *p, const u8 flags_last);
void player_item_check_pickup(struct scene *s, struct player *p);
void player_items_update(struct player *p, const struct input_parms iparms);
void player_item_draw(const struct player *p, const f32 subtick);

/*
 * Pistol
 */
void player_pistol_check_use(struct player *p,
			     const struct input_parms iparms);

/*
 * Bong
 */
void player_bong_check_use(struct player *p, const struct input_parms iparms);
void player_bong_weed_effect_update(struct player *p,
				    const u16 num_coughs_max);
void player_bong_cough_setup(struct player *p, u16 *num_coughs_max);
void player_bong_weed_effect_draw(const struct player *p,
				  const surface_t *surf, const u32 tick_cnt,
				  const u32 tick_cnt_last, const f32 subtick);

#endif /* _ENGINE_PLAYER_H_ */
