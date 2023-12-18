/**
 * @file include/engine/player.h
 */

#ifndef _ENGINE_PLAYER_H_
#define _ENGINE_PLAYER_H_

#include "engine/camera.h"
#include "engine/item.h"
#include "engine/collision_mesh.h"

/**
 * Which item is currently selected
 */
enum item_selected
{
	ITEM_SELECT_NONE = -1, ///< No Item
	ITEM_SELECT_PISTOL,    ///< Pistol Selected
	ITEM_SELECT_BONG,      ///< Bong Selected
	ITEM_SELECT_NITROUS,   ///< Nitrous Canister Selected
	ITEM_COUNT,            ///< Max Number of Items
};

/**
 * Flags for Which Items Player Has
 */
enum item_equipped_flags
{
	ITEM_HAS_NONE    = 0x0, ///< Empty Flag
	ITEM_HAS_PISTOL  = 0x1, ///< Pistol Flag
	ITEM_HAS_BONG    = 0x2, ///< Bong Flag
	ITEM_HAS_NITROUS = 0x4, ///< Nitrous Flag
};

/**
 * Enum for Which Drug the Player is On
 */
enum which_drug
{
	ON_DRUG_WEED,     ///< High on Weed
	ON_DRUG_NITROUS,  ///< High on Nitrous Oxide
	ON_DRUG_SHROOMS,  ///< Tripping on Shrooms
	ON_DRUG_KETAMINE, ///< High on Ketamine
	ON_DRUG_ALCOHOL,  ///< Drunk on Alcohol
	ON_DRUG_CRACK,    ///< High on Crack
	ON_DRUG_SALVIA,   ///< Has Special Gameplay Properties
	ON_DRUG_COUNT,    ///< Number of Possible Drug States
};

/**
 * Player Structure
 */
struct player
{
	struct camera view; ///< Camera for view matrix
	f32 pos[3]; ///< Position
	f32 vel[3]; ///< Current velocity
	f32 vel_last[3]; ///< Previous velocity
	u8 item_flags; ///< Which items player has
	s8 item_selected; ///< Item currently selected
	struct item items[ITEM_COUNT]; ///< Items array
	f32 recoil_amnt; ///< Current Recoil Amount
	f32 recoil_amnt_last; ///< Previous Recoil Amount
	f32 recoil_dir[2]; ///< Recoil direction
	f32 turn_offset[2]; ///< Current turn offset (Coughing, Recoil, etc.)
	f32 turn_offset_last[2]; ///< Previous turn offset
	f32 headbob_timer; ///< Current Headbob timer
	f32 headbob_timer_last; ///< Previous Headbob timer
	u8 which_drug; ///< Which drug player is on
	u16 drug_progress; ///< Drug progress
	u16 drug_duration; ///< Used as a percentage for `drug_progress`
};

/**
 * @defgroup engine Engine Functions
 * @{
 * @defgroup player Player Functions
 * @{
 */

/**
 * @defgroup player_main Player Main Functions
 * @{
 */
void player_init(const struct scene *s, struct player *p,
		 const u8 items_equipped_flags);
void player_terminate(struct player *p);
void player_update(struct scene *s, struct player *p,
		   const struct input_parms iparms);
/**
 * @}
 */

/**
 * @defgroup player_camera Player Camera Functions
 * @{
 */
void player_camera_view_matrix_setup(const struct player *p,
				     const f32 subtick);
void player_camera_look_update(struct player *p,
			       const struct input_parms iparms);
/**
 * @}
 */

/**
 * @defgroup player_physics Player Physics Functions
 * @{
 */
void player_friction(struct player *p);
void player_accelerate(struct player *p, const struct input_parms iparms);
/**
 * @}
 */

/**
 * @defgroup player_collision Player Collision Functions
 * @{
 */
void player_collision(const struct collision_mesh *m, struct player *p,
		      enum collision_type coltype);
/**
 * @}
 */

/**
 * @defgroup player_items Player Item Functions
 * @{
 */
void player_item_load(struct player *p, const u8 flags_last);
void player_item_check_pickup(struct scene *s, struct player *p);
void player_items_update(struct player *p, const struct input_parms iparms);
void player_item_draw(const struct player *p, const f32 subtick);
/**
 * @}
 */

/**
 * @defgroup player_pistol Player Pistol Functions
 * @{
 */
void player_pistol_check_use(struct player *p,
			     const struct input_parms iparms);
/**
 * @}
 */

/**
 * @defgroup player_weed Player Weed Functions
 * @{
 */
void player_bong_check_use(struct player *p, const struct input_parms iparms);
u16 player_bong_cough_setup(struct player *p);
void player_bong_weed_effect_update(struct player *p,
				    const u16 num_coughs_max);
void player_bong_weed_effect_draw(const struct player *p,
				  const surface_t *surf, const u32 tick_cnt,
				  const u32 tick_cnt_last, const f32 subtick);
/**
 * @}
 */

/**
 * @defgroup player_nitrous Player Nitrous Functions
 * @{
 */
void player_n2o_check_use(struct player *p, const struct input_parms iparms);
void player_n2o_effect_draw(const struct player *p, const surface_t *surf,
			    const u32 tick_cnt, const u32 tick_cnt_last,
			    const f32 subtick);
/**
 * @}
 */

/**
 * @defgroup player_drugs Player General Drug Functions
 * @{
 */
f32 player_drug_get_intensity(const struct player *p);

/**
 * @}
 * @}
 * @}
 */

#endif /* _ENGINE_PLAYER_H_ */
