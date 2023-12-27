#include <GL/gl.h>

#include "engine/sfx.h"
#include "engine/mesh.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

/**
 * Initializes a player struct.
 * @param[in] s Scene to initialize into
 * @param[out] p Player structure
 * @param[in] items_equipped_flags Which items player has in inventory
 */
void player_init(struct scene *s, struct player *p,
		 const u8 items_equipped_flags)
{
	camera_init(&p->view);
	vector_copy(p->pos, p->view.eye, 3);
	vector_zero(p->vel, 3);
	vector_zero(p->vel_last, 3);
	vector_zero(p->turn_offset, 2);
	vector_zero(p->turn_offset_last, 2);
	p->item_flags = items_equipped_flags;
	p->headbob_timer = 0;
	p->headbob_timer_last = 0;
	p->item_selected = ITEM_SELECT_NONE;
	p->area_index_last = 0;
	p->area_index = 0;
	player_item_load(p, ITEM_HAS_NONE);
	player_init_area_node_pointers(s, p);
	player_init_collision_by_area(s, p);
}

/**
 * Terminates a Player Struct
 * @param[out] p Player Structure
 */
void player_terminate(struct player *p)
{
	p->item_flags = ITEM_HAS_NONE;
	scene_destroy(&p->items[0].s);
}

/**
 * Updates a Player Structure
 * @param[in] s Scene to Refer to
 * @param[out] p Player Structure
 * @param[in] iparms Input Parameters
 */
void player_update(struct scene *s, struct player *p,
		   const struct input_parms iparms)
{
	vector_copy(p->view.angles_last, p->view.angles, 2);
	vector_copy(p->view.eye_last, p->view.eye, 3);
	vector_copy(p->turn_offset_last, p->turn_offset, 2);
	vector_copy(p->view.eye_last, p->pos, 3);
	vector_copy(p->vel_last, p->vel, 3);

	player_camera_look_update(p, iparms);
	player_friction(p);
	player_accelerate(p, iparms);
	vector_add(p->pos, p->vel, p->pos, 3);

	player_check_area_change(s, p);
	/*
	 * TODO: Refactor this
	 */
	player_collision(&p->floor_mesh, p, COLTYPE_FLOOR);
	player_collision(&p->walls_mesh, p, COLTYPE_WALLS);
	vector_copy(p->view.eye, p->pos, 3);

	player_item_check_pickup(s, p);
	struct item *bong = p->items + ITEM_SELECT_BONG;
	const u8 is_coughing = bong->qty2 > 0;
	const u8 must_stop_smoking = (bong->usage_timer >= 56) || iparms.up.z;
	static u16 num_coughs_max;

	if (must_stop_smoking && !is_coughing)
		num_coughs_max = player_bong_cough_setup(p);

	struct item *n2o = p->items + ITEM_SELECT_NITROUS;
	const u8 stop_huffing = n2o->usage_timer >= NITROUS_USAGE_TIMER_MAX;

	if (stop_huffing)
		player_n2o_trip_setup(p);

	player_bong_weed_effect_update(p, num_coughs_max);
	player_n2o_effect_update(p);
}
