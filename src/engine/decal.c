#include <malloc.h>
#include <GL/gl.h>

#include "engine/types.h"
#include "engine/util.h"
#include "engine/collision_mesh.h"
#include "engine/vertex.h"
#include "engine/vector.h"
#include "engine/texture.h"
#include "engine/decal.h"

static const u8 white[4] = {0xFF, 0xFF, 0xFF, 0xFF};

static u16 decals_vertex_buffer_count;
static s16 decal_head;
static struct vertex *decals_vertex_buffer;

static u32 decal_bullet_hole_id;

void decals_load(void)
{
	decal_bullet_hole_id =
		texture_create_file("rom:/bullet_hole.ia8.sprite");
}

void decal_add(const f32 *hit_pos, const f32 *normal, f32 *angle1)
{
	if (!decals_vertex_buffer)
		decals_vertex_buffer = malloc(0);

	if (decals_vertex_buffer_count < CONF_DECAL_MAX * 3)
	{
		decals_vertex_buffer_count += 3;
		decals_vertex_buffer = realloc(decals_vertex_buffer,
					       sizeof(struct vertex) *
					       decals_vertex_buffer_count);
	}

	decal_head++;
	if (decal_head > CONF_DECAL_MAX)
		decal_head = 1;

	s16 vind = (decal_head - 1) * 3;

	f32 angle2[3];

	debugf("%d\n", vind / 3);
	vector_normalize(angle1, 3);
	vector3_cross(angle1, normal, angle2);
	vec3 bottom_left, bottom_right, top_middle;
	vec3 angle2_half;

	vector_scale(angle1, 0.25f, angle1, 3);
	vector_scale(angle2, 0.25f, angle2, 3);
	vector_scale(angle2, 0.5f, angle2_half, 3);

	vector_sub(hit_pos, angle1, bottom_left, 3);
	vector_sub(bottom_left, angle2_half, bottom_left, 3);

	vector_add(hit_pos, angle1, bottom_right, 3);
	vector_sub(bottom_right, angle2_half, bottom_right, 3);

	vector_add(hit_pos, angle2, top_middle, 3);

	vector_copy(decals_vertex_buffer[vind + 0].pos, bottom_left, 3);
	vector_copy(decals_vertex_buffer[vind + 1].pos, bottom_right, 3);
	vector_copy(decals_vertex_buffer[vind + 2].pos, top_middle, 3);
	vector_copy(decals_vertex_buffer[vind + 0].uv, (f32[2]) {0, 0}, 2);
	vector_copy(decals_vertex_buffer[vind + 1].uv, (f32[2]) {1, 0}, 2);
	vector_copy(decals_vertex_buffer[vind + 2].uv, (f32[2]) {0.5f, 1}, 2);
	memcpy(decals_vertex_buffer[vind + 0].col, white, 4);
	memcpy(decals_vertex_buffer[vind + 1].col, white, 4);
	memcpy(decals_vertex_buffer[vind + 2].col, white, 4);
}

void decal_buffer_wipe(void)
{
	decal_head = 0;
	decals_vertex_buffer_count = 0;
	decals_vertex_buffer = realloc(decals_vertex_buffer, 0);
}

void decal_buffer_draw(void)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex),
			decals_vertex_buffer->pos);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex),
			  decals_vertex_buffer->uv);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct vertex),
		       decals_vertex_buffer->col);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, decal_bullet_hole_id);
	glDrawArrays(GL_TRIANGLES, 0, decals_vertex_buffer_count);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
