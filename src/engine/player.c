#include <GL/gl.h>

#include "engine/sfx.h"
#include "engine/mesh.h"
#include "engine/util.h"
#include "engine/vector.h"
#include "engine/player.h"

static struct collision_mesh floormesh;
static struct collision_mesh wallsmesh;

void player_init(const struct scene *s, struct player *p,
		 u8 items_equipped_flags)
{
	camera_init(&p->view);
	vector_copy(p->pos, p->view.eye, 3);
	vector_zero(p->vel, 3);
	vector_zero(p->vel_last, 3);
	vector_zero(p->turn_offset, 2);
	vector_zero(p->turn_offset_last, 2);
	p->item_flags = items_equipped_flags;
	p->headbob_timer = p->headbob_timer_last = 0;
	p->item_selected = ITEM_SELECT_NONE;
	player_item_load(p, ITEM_HAS_NONE);
	const struct mesh *floor_mesh =
		mesh_get_name(s->meshes, "Ground", s->num_meshes);
	const struct mesh *walls_mesh =
		mesh_get_name(s->meshes, "Walls", s->num_meshes);
	assertf(floor_mesh != NULL, "Floor Mesh not found\n");
	assertf(walls_mesh != NULL, "Wall Mesh not found\n");

	collision_mesh_gen(&floormesh, floor_mesh);
	collision_mesh_gen(&wallsmesh, walls_mesh);
}

void player_terminate(struct player *p)
{
	p->item_flags = ITEM_HAS_NONE;
	scene_destroy(&p->items[0].s);
}

void player_update(struct scene *s, struct player *p,
		   const struct input_parms iparms)
{
	static u16 num_coughs_max;

	vector_copy(p->view.angles_last, p->view.angles, 2);
	vector_copy(p->view.eye_last, p->view.eye, 3);
	vector_copy(p->turn_offset_last, p->turn_offset, 2);
	vector_copy(p->view.eye_last, p->pos, 3);
	vector_copy(p->vel_last, p->vel, 3);

	player_camera_look_update(p, iparms);
	player_friction(p);
	player_accelerate(p, iparms);
	vector_add(p->pos, p->vel, p->pos, 3);

	player_collision(&floormesh, p, COLTYPE_FLOOR);
	player_collision(&wallsmesh, p, COLTYPE_WALLS);
	vector_copy(p->view.eye, p->pos, 3);

	player_item_check_pickup(s, p);
	struct item *bong = p->items + 1;
	const u8 is_coughing = bong->qty2 > 0;
	const u8 must_stop_smoking = (bong->usage_timer >= 56) || iparms.up.z;

	if (must_stop_smoking && !is_coughing)
		player_bong_cough_setup(p, &num_coughs_max);

	player_bong_weed_effect_update(p, num_coughs_max);
}
