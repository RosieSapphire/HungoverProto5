#include "engine/vector.h"
#include "engine/raycast.h"
#include "engine/player.h"

#define HEIGHT 2.1f
#define GIRTH  1.8f

static const f32 upward[3] = {0, 1, 0}, downward[3] = {0, -1, 0};
static const f32 floor_check_extra[3] = {0, 0.5f, 0};

static void _player_collision_floor(struct player *p, vec3 *v, vec3 n,
				    vec3 eye, vec3 dir, f32 *dist)
{
	f32 push_vec[3], push;

	vector_add(eye, floor_check_extra, eye, 3);
	if (vector_dot(n, upward, 3) < 0.0f)
		return;

	vector_copy(dir, n, 3);
	vector_scale(dir, -1, 3);
	if (!raycast_triangle(eye, dir, v, dist))
		return;

	const f32 push_amnt = HEIGHT - *dist + 0.5f;

	/*
	 * If you're on a slope, it will snap you to it
	 */
	push = push_amnt;
	if (vector_dot(dir, downward, 3) == 1.0f)
		push = fmaxf(push, 0);

	vector_copy(push_vec, n, 3);
	vector_scale(push_vec, push, 3);
	vector_add(p->pos, push_vec, p->pos, 3);
}

static void _player_collision_walls(struct player *p, vec3 *v, vec3 n,
				    vec3 eye, vec3 dir, f32 *dist)
{
	f32 push_vec[3], push;

	vector_copy(dir, n, 3);
	vector_scale(dir, -1, 3);
	if (!raycast_triangle(eye, dir, v, dist))
		return;

	const f32 push_amnt = GIRTH - *dist;

	/*
	 * If you're on a slope, it will snap you to it
	 */
	push = fmaxf(push_amnt, 0);
	vector_copy(push_vec, n, 3);
	vector_scale(push_vec, push, 3);
	vector_add(p->pos, push_vec, p->pos, 3);
}

static void (*colfuncs[2])(struct player *, vec3 *,
			   vec3, vec3, vec3, f32 *) = {
	_player_collision_floor, _player_collision_walls,
};

void player_collision(const struct collision_mesh *m, struct player *p,
		      enum collision_type coltype)
{
	f32 dir[3], eye[3], dist;

	for (u16 i = 0; i < m->num_verts / 3; i++)
	{
		/*
		 * Generating the normals
		 */
		vec3 v[3], a, b, n;

		vector_copy(v[0], m->verts[i * 3 + 0], 3);
		vector_copy(v[1], m->verts[i * 3 + 1], 3);
		vector_copy(v[2], m->verts[i * 3 + 2], 3);

		vector_sub(v[1], v[0], a, 3);
		vector_sub(v[2], v[0], b, 3);
		vector3_cross(a, b, n);
		vector_normalize(n, 3);
		vector_copy(eye, p->pos, 3);

		/*
		 * Determining Collision Type
		 */
		assertf(coltype < COLTYPE_COUNT, "Invalid Collision Type.\n");
		(*colfuncs[coltype])(p, v, n, eye, dir, &dist);
	}
}
