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
}

void player_update_coughing(struct player *p, const struct input_parms iparms,
			    u16 *num_coughs_max)
{
	struct item *bong = p->items + 1;
	const u8 is_coughing = bong->qty2 > 0;
	const u8 must_stop_smoking = (bong->usage_timer >= 56) || iparms.up.z;

	if (!must_stop_smoking || is_coughing)
		return;

	u16 cough_rand = 0;

	if (bong->usage_timer_last)
	{
		f32 usage_timer_exp = ((f32)bong->usage_timer_last / 56.0f);

		usage_timer_exp *= usage_timer_exp * usage_timer_exp;
		usage_timer_exp *= 56;

		if ((u16)usage_timer_exp)
			cough_rand = (rand() % (u16)usage_timer_exp);

		if (bong->usage_timer_last >= 48 &&
			bong->usage_timer_last <= 50)
		{
			mixer_ch_set_vol(SFXC_ITEM1, 0.3f, 0.3f);
			wav64_play(&bong_hit_good_sfx, SFXC_ITEM1);
			cough_rand = 0;
		}
		else
		{
			mixer_ch_set_vol(SFXC_ITEM1, 0.3f, 0.3f);
			wav64_play(&bong_hit_bad_sfx, SFXC_ITEM1);
		}

		p->weed_high_amnt = 1.0f;
		p->weed_progress = 0;
		p->weed_duration = 240;
		mixer_ch_set_vol(SFXC_MUSIC0, 0, 0);
		mixer_ch_set_freq(SFXC_MUSIC0, 22050);
		wav64_play(&nitrous_whine, SFXC_MUSIC0);
	}

	bong->qty2 = cough_rand;
	*num_coughs_max = bong->qty2;
}
