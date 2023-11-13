#ifndef _ENGINE_TEXTURE_H_
#define _ENGINE_TEXTURE_H_

#include <libdragon.h>

#include "engine/types.h"

/**
 * struct texture - Texture Struct
 * @id: OpenGL Texture ID
 * @spr: Libdragon Sprite
 */
struct texture
{
	u32 id;
	sprite_t *spr;
};

struct texture texture_create_file(const char *path);

#endif /* _ENGINE_TEXTURE_H_ */
