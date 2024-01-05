#include <stdlib.h>
#include <GL/gl.h>
#include <libdragon.h>

#include "engine/util.h"
#include "engine/vertex.h"
#include "engine/vector.h"
#include "engine/raycast.h"
#include "engine/profiler.h"
#include "engine/particles.h"

void particle_emitter_init(struct particle_emitter *e,
			   const u16 emit_timer_max, const f32 *spawn_pos)
{
	e->num_particles = 0;
	e->particles = malloc(0);
	e->emit_timer_last = 0;
	e->emit_timer = 0;
	e->emit_timer_max = emit_timer_max;
	if (spawn_pos)
		vector_copy(e->spawn_pos, spawn_pos, 3);
}

void particle_emitter_spawn(struct particle_emitter *e,
			    const f32 *pos, const f32 *dir)
{
	struct particle *partcur;

	e->particles = realloc(e->particles,
			       sizeof(struct particle) * ++e->num_particles);
	partcur = e->particles + e->num_particles - 1;
	vector_copy(partcur->pos, pos ? pos : e->spawn_pos, 3);
	vector_copy(partcur->pos_last, partcur->pos, 3);
	partcur->lifetime = 0;

	const f32 randvec[3] = {
		(randf01() * 2) - 1,
		(randf01() * 2) - 1,
		(randf01() * 2) - 1,
	};

	if (dir)
	{
		vector_copy(partcur->vel, dir, 3);
		vector_add(partcur->vel, randvec, partcur->vel, 3);
		vector_normalize(partcur->vel, 3);
		return;
	}

	vector_copy(partcur->vel, randvec, 3);
	vector_normalize(partcur->vel, 3);
}

static void _particle_update(struct particle *p,
			     const struct collision_mesh *colmesh)
{
	if (++p->lifetime >= CONF_PARTICLE_MAX_LIFETIME)
		return;

	vector_copy(p->pos_last, p->pos, 3);
	vector_add(p->vel, (f32[3]) {0, -0.412346f, 0}, p->vel, 3);
	vector_add(p->pos, p->vel, p->pos, 3);

	for (u16 i = 0; i < colmesh->num_verts;)
	{
		vec3 v[3], a, b, n, eye, dir;
		f32 dist;

		vector_copy((f32 *)(v + 0), (f32 *)(colmesh->verts + i++), 3);
		vector_copy((f32 *)(v + 1), (f32 *)(colmesh->verts + i++), 3);
		vector_copy((f32 *)(v + 2), (f32 *)(colmesh->verts + i++), 3);
		vector_sub((f32 *)(v + 1), (f32 *)(v + 0), a, 3);
		vector_sub((f32 *)(v + 2), (f32 *)(v + 0), b, 3);
		vector3_cross(a, b, n);
		vector_normalize(n, 3);

		vector_add(p->pos, (vec3) {0, 2, 0}, eye, 3);
		vector_scale(n, -1, dir, 3);

		if (!raycast_triangle(eye, dir, v, &dist))
			continue;

		const f32 push_amnt = fmaxf(2.0f - dist, 0);
		vec3 push;

		if (push_amnt <= 0.0f)
			continue;

		vector_scale(n, push_amnt, push, 3);
		vector_add(p->pos, push, p->pos, 3);
		vector_scale(p->vel, 0.6f, p->vel, 3);
		p->vel[1] *= -1;
		break;
	}
}

void particle_emitter_update(struct particle_emitter *e,
			     const struct collision_mesh *colmesh)
{
	for (u16 i = 0; i < e->num_particles; i++)
		_particle_update(e->particles + i, colmesh);

	if (!e->emit_timer_max)
		return;

	e->emit_timer_last = e->emit_timer++;
	if (e->emit_timer >= e->emit_timer_max)
	{
		particle_emitter_spawn(e, e->spawn_pos, NULL);
		e->emit_timer = 0;
	}
}

void particle_emitter_draw(const struct particle_emitter *e,
			   const f32 subtick)
{
	u16 active_particles = 0;
	u16 *active_particle_indis = malloc(0);

	for (u16 i = 0; i < e->num_particles; i++)
	{
		if (e->particles[i].lifetime >= CONF_PARTICLE_MAX_LIFETIME)
			continue;

		active_particle_indis = realloc(active_particle_indis,
						sizeof(u16) *
						++active_particles);
		active_particle_indis[active_particles - 1] = i;
	}

	if (!active_particles)
	{
		free(active_particle_indis);
		return;
	}

	struct vertex *vertex_buffer = malloc(sizeof(struct vertex) *
					      active_particles);

	for (u16 i = 0; i < active_particles; i++)
	{
		const struct particle *p =
			e->particles + active_particle_indis[i];
		f32 p_pos_lerp[3];

		vector_lerp(p->pos_last, p->pos, subtick, p_pos_lerp, 3);
		vector_copy(vertex_buffer[i].pos, p_pos_lerp, 3);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), vertex_buffer);
	glPointSize(2.4f);
	glColor3f(1, 1, 1);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_POINTS, 0, active_particles);
	glDisableClientState(GL_VERTEX_ARRAY);

	free(vertex_buffer);
	free(active_particle_indis);
}

void particle_emitter_terminate(struct particle_emitter *e)
{
	e->num_particles = 0;
	free(e->particles);
	e->emit_timer = 0;
	e->emit_timer_max = 0;
	vector_copy(e->spawn_pos, (f32[3]) {0, 0, 0}, 3);
}
