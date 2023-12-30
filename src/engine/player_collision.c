#include "engine/util.h"
#include "engine/vector.h"
#include "engine/raycast.h"
#include "engine/decal.h"
#include "engine/player.h"

#define HEIGHT 2.1f
#define GIRTH  1.8f

static u16 num_area_nodes = 0;
static struct node **area_nodes = NULL;
static const f32 upward[3] = {0, 1, 0}, downward[3] = {0, -1, 0};
static const f32 floor_check_extra[3] = {0, 0.5f, 0};

void player_init_area_node_pointers(struct scene *s, struct player *p)
{
	if (!area_nodes)
		area_nodes = malloc(0);

	while (num_area_nodes < CONF_MAX_NUM_AREAS)
	{
		char str[16];
		struct node *n;

		sprintf(str, "Area.%d", num_area_nodes);
		n = node_children_find(&s->root_node, str);

		if (!n)
			break;

		area_nodes = realloc(area_nodes,
				     sizeof(struct node *) * ++num_area_nodes);
		area_nodes[num_area_nodes - 1] = n;
	}

	for (u16 i = 0; i < num_area_nodes; i++)
		node_tree_toggle_meshes(s->meshes, s->num_meshes,
					area_nodes[i], 0);

	node_tree_toggle_meshes(s->meshes, s->num_meshes,
				area_nodes[p->area_index], 1);
}

void player_init_collision_by_area(struct scene *s, struct player *p)
{
	char floor_mesh_name[CONF_NAME_MAX];
	char walls_mesh_name[CONF_NAME_MAX];
	const struct mesh *floor_mesh;
	const struct mesh *walls_mesh;

	node_tree_toggle_meshes(s->meshes, s->num_meshes,
				area_nodes[p->area_index_last], 0);
	node_tree_toggle_meshes(s->meshes, s->num_meshes,
				area_nodes[p->area_index], 1);

	snprintf(floor_mesh_name, CONF_NAME_MAX - 1,
		 "Ground.%d", p->area_index);
	snprintf(walls_mesh_name, CONF_NAME_MAX - 1,
		 "Walls.%d", p->area_index);
	debugf("%s, %s\n", floor_mesh_name, walls_mesh_name);
	floor_mesh = mesh_get_name(s->meshes, floor_mesh_name, s->num_meshes);
	walls_mesh = mesh_get_name(s->meshes, walls_mesh_name, s->num_meshes);
	assertf(floor_mesh, "Floor Mesh not found\n");
	assertf(walls_mesh, "Wall Mesh not found\n");

	collision_mesh_free(&p->floor_mesh);
	collision_mesh_free(&p->walls_mesh);
	collision_mesh_gen(&p->floor_mesh, floor_mesh);
	collision_mesh_gen(&p->walls_mesh, walls_mesh);
}

/**
 * Handles Player Collision with Floor Triangle
 * @param[in,out] p Player to Handle Collision for
 * @param[in] v 3 Vertices for the Floor Triangle
 * @param[in] n Normal Vector of Triangle
 * @param[in] eye Raycast Eye
 * @param[in] dir Raycast Direction
 * @param[out] dist Pointer to Output Distance from Triangle
 */
static void _player_collision_floor(struct player *p, vec3 *v, vec3 n,
				    vec3 eye, vec3 dir, f32 *dist)
{
	f32 push_vec[3], push;

	vector_add(eye, floor_check_extra, eye, 3);
	if (vector_dot(n, upward, 3) < 0.0f)
		return;

	vector_copy(dir, n, 3);
	vector_scale(dir, -1, dir, 3);
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
	vector_scale(push_vec, push, push_vec, 3);
	vector_add(p->pos, push_vec, p->pos, 3);
}

/**
 * Handles Player Collision with Wall Triangle
 * @param[in,out] p Player to Handle Collision for
 * @param[in] v 3 Vertices for the Wall Triangle
 * @param[in] n Normal Vector of Triangle
 * @param[in] eye Raycast Eye
 * @param[in] dir Raycast Direction
 * @param[out] dist Pointer to Output Distance from Triangle
 */
static void _player_collision_walls(struct player *p, vec3 *v, vec3 n,
				    vec3 eye, vec3 dir, f32 *dist)
{
	f32 push_vec[3], push;

	vector_copy(dir, n, 3);
	vector_scale(dir, -1, dir, 3);
	if (!raycast_triangle(eye, dir, v, dist))
		return;

	const f32 push_amnt = GIRTH - *dist;

	/*
	 * If you're on a slope, it will snap you to it
	 */
	push = fmaxf(push_amnt, 0);
	vector_copy(push_vec, n, 3);
	vector_scale(push_vec, push, push_vec, 3);
	vector_add(p->pos, push_vec, p->pos, 3);
}

static void (*colfuncs[2])(struct player *, vec3 *,
			   vec3, vec3, vec3, f32 *) = {
	_player_collision_floor, _player_collision_walls,
};

/**
 * Handles All Player Collisions
 * @param[in] m Collision Mesh
 * @param[in,out] p Player to Handle Collision for
 * @param[in] coltype Collision Type (Floors, Walls, etc.)
 */
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

void player_check_area_change(struct scene *s, struct player *p)
{
	static u8 has_collided_last;
	static u8 has_collided;

	has_collided_last = has_collided;

	for (u16 i = 0; i < num_area_nodes; i++)
	{
		f32 dist_vec[3];
		f32 dist;

		if (i == 0)
			continue;

		vector_sub(area_nodes[i]->children->trans[3],
			   p->view.eye, dist_vec, 3);
		dist = vector_magnitude_sqr(dist_vec, 3);
		has_collided = dist < 8.0f;

		if (has_collided && !has_collided_last)
		{
			decal_buffer_wipe();
			if (i == p->area_index)
			{
				const u16 tmp = p->area_index_last;

				p->area_index_last = i;
				p->area_index = tmp;
				debugf("%u, %u\n", p->area_index_last, p->area_index);
				break;
			}
			p->area_index_last = p->area_index;
			p->area_index = i;
			debugf("%u, %u\n", p->area_index_last, p->area_index);
			break;
		}
	}

	if (p->area_index == p->area_index_last)
		return;

	player_init_collision_by_area(s, p);
}
