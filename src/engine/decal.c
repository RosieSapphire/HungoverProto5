#include <malloc.h>
#include <GL/gl.h>

#include "engine/types.h"
#include "engine/collision_mesh.h"
#include "engine/vertex.h"
#include "engine/vector.h"
#include "engine/decal.h"

static u16 decals_vertex_buffer_count;
static u16 decal_head;
static struct vertex *decals_vertex_buffer;

void decal_add(const f32 *hit_pos, const f32 *normal, f32 *angle1)
{
	if (!decals_vertex_buffer)
		decals_vertex_buffer = malloc(0);

	decals_vertex_buffer_count += 3;
	decals_vertex_buffer = realloc(decals_vertex_buffer,
				       sizeof(struct vertex) *
				       decals_vertex_buffer_count);
	decal_head++;
	const u16 vind = (decal_head - 1) * 3;
	f32 angle2[3];

	vector_normalize(angle1, 3);
	vector3_cross(angle1, normal, angle2);
	vec3 bottom_left, bottom_right, top_middle;

	vector_add(hit_pos, angle1, bottom_left, 3);
	vector_sub(hit_pos, angle1, bottom_right, 3);
	vector_add(hit_pos, angle2, top_middle, 3);
	vector_copy(decals_vertex_buffer[vind + 0].pos, bottom_left, 3);
	vector_copy(decals_vertex_buffer[vind + 1].pos, bottom_right, 3);
	vector_copy(decals_vertex_buffer[vind + 2].pos, top_middle, 3);
}

void decal_buffer_draw(void)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex),
			decals_vertex_buffer);
	glDrawArrays(GL_TRIANGLES, 0, decals_vertex_buffer_count);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
