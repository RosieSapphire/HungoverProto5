#ifndef _ENGINE_PARTICLES_H_
#define _ENGINE_PARTICLES_H_

#include "engine/types.h"
#include "engine/config.h"
#include "engine/collision_mesh.h"

struct particle
{
	f32 pos_last[3];
	f32 pos[3];
	f32 vel[3];
	u16 lifetime;
};

struct particle_emitter
{
	u16 num_particles;
	struct particle *particles;
	u16 emit_timer_last;
	u16 emit_timer;
	u16 emit_timer_max;
	f32 spawn_pos[3];
};

void particle_emitter_init(struct particle_emitter *e,
			   const u16 emit_timer_max, const f32 *spawn_pos);
void particle_emitter_spawn(struct particle_emitter *e,
			    const f32 *pos, const f32 *dir);
void particle_emitter_update(struct particle_emitter *e,
			     const struct collision_mesh *colmesh);
void particle_emitter_draw(const struct particle_emitter *e,
			   const f32 subtick);
void particle_emitter_terminate(struct particle_emitter *e);

#endif /* _ENGINE_PARTICLES_H_ */
