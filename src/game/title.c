#include <GL/gl.h>

#include "engine/util.h"
#include "engine/texture.h"
#include "engine/mesh.h"

#include "game/title.h"

static struct texture title_text;
static struct mesh *text_mesh;
static f32 timer_last, timer;

/**
 * title_load - Initializes Title Screen
 */
void title_load(void)
{
	const struct vertex verts[] = {
		{{-64, -32, 0}, {0.0f, 1.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
		{{ 64, -32, 0}, {1.0f, 1.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
		{{-64,  32, 0}, {0.0f, 0.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
		{{ 64,  32, 0}, {1.0f, 0.0f}, {0xFF, 0xFF, 0xFF, 0xFF}},
	};

	const u16 indis[] = {0, 1, 2, 2, 1, 3};

	title_text = texture_create_file("rom:/title_text.ia8.sprite");
	text_mesh = mesh_create_data("Test", 4, 6, verts, indis);

	timer_last = timer = 0.0f;
}

/**
 * title_unload - Unloads Title Screen
 */
void title_unload(void)
{
	timer_last = timer = 0.0f;
	mesh_destroy(text_mesh);
}

/**
 * title_update - Updates the Title Screen
 */
void title_update(void)
{
	timer_last = timer;
	timer += 0.1f;
}

/**
 * _title_setup_wiggle - Sets up Model Matrix with Wiggle for Title
 * @subtick: Delta Value between Frames
 */
static void _title_setup_wiggle(f32 subtick)
{
	const f32 timer_lerp = lerpf(timer_last, timer, subtick);
	const f32 pos_off[3] = {
		sinf(timer_lerp) * 3,
		sinf(timer_lerp * 1.5f) * 3,
		-128.0f,
	};
	const f32 rot_off = sinf(timer_lerp) * 4;

	glTranslatef(pos_off[0], pos_off[1], pos_off[2]);
	glRotatef(rot_off, 0, 0, 1);

}

/**
 * title_draw - Draws the Title Screen
 * @subtick: Delta Value between Frames
 */
void title_draw(f32 subtick)
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex),
		 text_mesh->verts->pos);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex),
		 text_mesh->verts->uv);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct vertex),
		 text_mesh->verts->col);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, title_text.id);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_title_setup_wiggle(subtick);

	glDrawElements(GL_TRIANGLES, text_mesh->num_indis,
		GL_UNSIGNED_SHORT, text_mesh->indis);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}
