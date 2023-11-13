#include <stdio.h>
#include <GL/gl.h>

#include "engine/texture.h"

/**
 * texture_create_file - Creates Texture from File
 * @path: Path to Load from
 *
 * Return: Loaded Texture
 */
struct texture texture_create_file(const char *path)
{
	struct texture t;

	t.spr = sprite_load(path);
	if (!t.spr)
	{
		debugf("Failed to load texture from '%s'\n", path);
		t.id = -1;
		return (t);
	}

	glGenTextures(1, &t.id);
	glBindTexture(GL_TEXTURE_2D, t.id);
	glSpriteTextureN64(GL_TEXTURE_2D, t.spr, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	return (t);
}

/**
 * texture_destroy - Destroys a Texture
 * @t: Texture to Destroy
 */
void texture_destroy(struct texture *t)
{
	glDeleteTextures(1, &t->id);
	sprite_free(t->spr);
}
