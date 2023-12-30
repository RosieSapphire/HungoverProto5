#include <malloc.h>
#include <GL/gl.h>
#include <libdragon.h>

#include "engine/util.h"
#include "engine/vertex.h"
#include "engine/vector.h"
#include "engine/particles.h"

void particle_emitter_init(struct particle_emitter *e,
			   const u16 emit_timer_max, const f32 *pos)
{
	e->num_particles = 0;
	e->particles = malloc(0);
	e->emit_timer_last = 0;
	e->emit_timer = 0;
	e->emit_timer_max = emit_timer_max;
	vector_copy(e->pos, pos, 3);
}

static void _particle_emitter_spawn(struct particle_emitter *e)
{
	struct particle *partcur;

	e->particles = realloc(e->particles,
			       sizeof(struct particle) * ++e->num_particles);
	partcur = e->particles + e->num_particles - 1;
	vector_copy(partcur->pos, e->pos, 3);
	vector_copy(partcur->pos_last, e->pos, 3);

	f32 vel[3] = {
		(randf01() * 2) - 1, (randf01() * 2) - 1, (randf01() * 2) - 1,
	};

	vector_normalize(vel, 3);
	vector_copy(partcur->vel, vel, 3);
}

static void _particle_update(struct particle *p)
{
	vector_copy(p->pos_last, p->pos, 3);
	vector_add(p->vel, (f32[3]) {0, -0.25f, 0}, p->vel, 3);
	vector_add(p->pos, p->vel, p->pos, 3);
}

void particle_emitter_update(struct particle_emitter *e)
{
	for (u16 i = 0; i < e->num_particles; i++)
		_particle_update(e->particles + i);

	e->emit_timer_last = e->emit_timer++;
	if (e->emit_timer >= e->emit_timer_max)
	{
		_particle_emitter_spawn(e);
		e->emit_timer = 0;
	}
}

void particle_emitter_draw(const struct particle_emitter *e,
			   const f32 subtick)
{
	struct vertex *vertex_buffer = malloc(sizeof(struct vertex) *
					      e->num_particles);

	debugf("%d\n", e->num_particles);
	for (u16 i = 0; i < e->num_particles; i++)
	{
		const struct particle *p = e->particles + i;
		f32 p_pos_lerp[3];

		vector_lerp(p->pos_last, p->pos, subtick, p_pos_lerp, 3);
		vector_copy(vertex_buffer[i].pos, p_pos_lerp, 3);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), vertex_buffer);
	glPointSize(2.4f);
	glColor3f(1, 1, 1);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_POINTS, 0, e->num_particles);
	glDisableClientState(GL_VERTEX_ARRAY);

	free(vertex_buffer);
}

void particle_emitter_terminate(struct particle_emitter *e)
{
	e->num_particles = 0;
	free(e->particles);
	e->emit_timer = 0;
	e->emit_timer_max = 0;
	vector_copy(e->pos, (f32[3]) {0, 0, 0}, 3);
}
