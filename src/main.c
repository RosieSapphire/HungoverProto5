#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <libdragon.h>

#include "engine/config.h"
#include "engine/types.h"
#include "engine/texture.h"
#include "engine/mesh.h"

static surface_t *color_buffer;
static surface_t depth_buffer;
static struct texture test_texture;
static struct mesh *test_mesh;


/**
 * _init - Initialization Function
 */
static void _init(void)
{
	display_init((resolution_t){CONF_WIDTH, CONF_HEIGHT, 0},
	      DEPTH_32_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
	rdpq_init();
	dfs_init(DFS_DEFAULT_LOCATION);
	debug_init_isviewer();
	debug_init_usblog();

	gl_init();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-CONF_NEAR * CONF_ASPECT, CONF_NEAR * CONF_ASPECT,
	   -CONF_NEAR, CONF_NEAR, CONF_NEAR, CONF_FAR);

	depth_buffer = surface_alloc(FMT_RGBA16, 320, 240);
	test_texture = texture_create_file("rom:/title_text.ia8.sprite");

	const struct vertex verts[] = {
		{{-64, -32, 0}, {0.0f, 1.0f}, {0xFF, 0x00, 0x00, 0xFF}},
		{{ 64, -32, 0}, {1.0f, 1.0f}, {0x00, 0xFF, 0x00, 0xFF}},
		{{-64,  32, 0}, {0.0f, 0.0f}, {0x00, 0x00, 0xFF, 0xFF}},
		{{ 64,  32, 0}, {1.0f, 0.0f}, {0x00, 0x00, 0x00, 0xFF}},
	};

	const u16 indis[] = {0, 1, 2, 2, 1, 3};

	test_mesh = mesh_create_data("Test", 4, 6, verts, indis);
}

/**
 * _draw - Drawing Function
 */
static void _draw(void)
{
	color_buffer = display_get();
	rdpq_attach(color_buffer, &depth_buffer);
	gl_context_begin();
	glClearColor(1.0f, 0.5f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex),
		 test_mesh->verts->pos);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex),
		 test_mesh->verts->uv);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct vertex),
		 test_mesh->verts->col);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, test_texture.id);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	static u32 time;
	glTranslatef(0, 0, -128);
	glRotatef(time++ * 2, 0, 1, 0);

	glDrawElements(GL_TRIANGLES, test_mesh->num_indis,
		GL_UNSIGNED_SHORT, test_mesh->indis);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	gl_context_end();
	rdpq_detach_show();
}

/**
 * main - Main Function
 *
 * Return: 0 = Success
 */
int main(void)
{
	_init();

	while (1)
	{
		_draw();
	}

	return (0);
}
