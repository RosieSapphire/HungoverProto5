#include <GL/gl.h>

#include "engine/sfx.h"
#include "engine/vector.h"
#include "engine/raycast.h"
#include "engine/player.h"

static f32 hit_pos[3];
static struct collision_mesh decal_mesh;
static u16 decal_head = 0;

static void _player_pistol_check_hit(struct player *p)
{
	f32 dir[3] = {0}, eye[3] = {0}, focus[3] = {0}, dist;

	vector_copy(eye, p->view.eye, 3);
	camera_get_focus_now(&p->view, focus);
	vector_sub(focus, eye, dir, 3);
	vector_normalize(dir, 3);

	for (u16 i = 0; i < p->walls_mesh.num_verts / 3; i++)
	{
		vec3 v[3], a, b, n;

		vector_copy(v[0], p->walls_mesh.verts[i * 3 + 0], 3);
		vector_copy(v[1], p->walls_mesh.verts[i * 3 + 1], 3);
		vector_copy(v[2], p->walls_mesh.verts[i * 3 + 2], 3);

		vector_sub(v[1], v[0], a, 3);
		vector_sub(v[2], v[0], b, 3);
		vector3_cross(a, b, n);
		vector_normalize(n, 3);
		vector_copy(eye, p->pos, 3);

		if (!decal_mesh.verts)
			decal_mesh.verts = malloc(0);

		if (raycast_triangle(eye, dir, v, &dist))
		{
			vector_copy(hit_pos, dir, 3);
			vector_scale(hit_pos, dist, 3);
			vector_add(hit_pos, eye, hit_pos, 3);

			decal_mesh.num_verts += 3;
			decal_mesh.verts = realloc(decal_mesh.verts,
						   sizeof(struct vertex) *
						   decal_mesh.num_verts);
			decal_head++;
			const u16 vind = (decal_head - 1) * 3;
			f32 axis_a[3];
			f32 axis_b[3];

			vector_copy(axis_a, a, 3);
			vector_normalize(axis_a, 3);
			vector3_cross(axis_a, n, axis_b);

			vec3 bottom_left, bottom_right, top_middle;

			vector_add(hit_pos, axis_a, bottom_left, 3);
			vector_sub(hit_pos, axis_a, bottom_right, 3);
			vector_add(hit_pos, axis_b, top_middle, 3);

			vector_copy(decal_mesh.verts[vind + 0],
				    bottom_left, 3);
			vector_copy(decal_mesh.verts[vind + 1],
				    bottom_right, 3);
			vector_copy(decal_mesh.verts[vind + 2],
				    top_middle, 3);
			break;
		}
	}
}

/**
 * Checks if player is using pistol.
 * @param[in,out] p Player
 * @param[in] iparms Input Parameters
 */
void player_pistol_check_use(struct player *p,
			     const struct input_parms iparms)
{
	struct item *pistol = p->items + 0;

	if (!iparms.press.z)
		return;

	if (!item_anim_at_end(pistol, 0) && !item_anim_is_index(pistol, 1))
		return;

	u8 has_loaded = pistol->qty1 > 0;
	u8 has_reserve = pistol->qty2 > 0;

	if (has_loaded)
	{
		pistol->anim_index = 1;
		pistol->qty1--;
		scene_anims_set_flags(&pistol->s, ANIM_IS_PLAYING);
		scene_anims_set_frame(&pistol->s, 0);
		mixer_ch_set_vol(SFXC_ITEM1, 0.8f, 0.8f);
		wav64_play(&pistol_fire_sfx, SFXC_ITEM1);
		p->recoil_amnt += 0.2f;
		vector_copy(p->recoil_dir, (f32[2]) {
			(f32)((rand() % 255) - 127) / 128.0f,
			(f32)((rand() % 255) - 127) / 128.0f},
			2);
		_player_pistol_check_hit(p);
	}
	else if (has_reserve && !has_loaded)
	{
		pistol->qty2 -= 17;
		pistol->qty1 += 17;
	}
	else if (!has_reserve && !has_loaded)
	{
		mixer_ch_set_vol(SFXC_ITEM0, 0.5f, 0.5f);
		wav64_play(&pistol_click_sfx, SFXC_ITEM0);
	}
}

void player_pistol_decal_draw(__attribute__((unused))const struct player *p)
{
	const struct collision_mesh *m = &decal_mesh;

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(vec3), m->verts);
	glDrawArrays(GL_TRIANGLES, 0, decal_mesh.num_verts);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
