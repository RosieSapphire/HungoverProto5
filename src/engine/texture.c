/**
 * @file src/engine/texture.c
 */

#include <string.h>
#include <malloc.h>
#include <GL/gl.h>

#include "engine/texture.h"

u16 num_texs_loaded; ///< Number of textures loaded
char tex_paths_loaded[MAX_TOTAL_TEXS][TEX_PATH_MAX_LEN]; ///< Texture paths
struct texture *tex_objs_loaded; ///< Texture objects

/**
 * Initializes Texture Subsystem
 */
void textures_init(void)
{
	memset(tex_paths_loaded, 0, MAX_TOTAL_TEXS * TEX_PATH_MAX_LEN);
	tex_objs_loaded = malloc(0);
}

/**
 * Loads a Texture from a File
 * @param[in] path Texture Path
 * @return OpenGL ID for Texture at Index
 */
u32 texture_create_file(const char *path)
{
	if (!strncmp(path, "None", 4))
		return (0);

	struct texture t;

	for (u16 i = 0; i < num_texs_loaded; i++)
		if (!strcmp(path, tex_paths_loaded[i]))
			return (tex_objs_loaded[i].id);

	num_texs_loaded++;
	tex_objs_loaded = realloc(tex_objs_loaded,
				  sizeof(struct texture) * num_texs_loaded);
	t.spr = sprite_load(path);

	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	rdpq_texparms_t parms = {
		.s.repeats = REPEAT_INFINITE,
		.t.repeats = REPEAT_INFINITE
	};

	glSpriteTextureN64(GL_TEXTURE_2D, t.spr, &parms);
	glBindTexture(GL_TEXTURE_2D, 0);

	strcpy(tex_paths_loaded[num_texs_loaded - 1], path);
	tex_objs_loaded[num_texs_loaded - 1] = t;
	return (tex_objs_loaded[num_texs_loaded - 1].id);
}

/**
 * Destroys a Texture
 * @param[in,out] t Texture in Question
 */
void texture_destroy(struct texture *t)
{
	glDeleteTextures(1, &t->id);
	sprite_free(t->spr);
	num_texs_loaded--;
	tex_objs_loaded = realloc(tex_objs_loaded,
				  sizeof(struct texture) * num_texs_loaded);
}
