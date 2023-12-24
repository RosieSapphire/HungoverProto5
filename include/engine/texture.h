#ifndef _ENGINE_TEXTURE_H_
#define _ENGINE_TEXTURE_H_

/**
 * @file include/engine/texture.h
 */

#include <libdragon.h>

#include "engine/types.h"
#include "engine/config.h"

/**
 * Texture Struct
 */
struct texture
{
	u32 id; ///< OpenGL Texture ID
	sprite_t *spr; ///< Libdragon Sprite
};

extern u16 num_texs_loaded;
extern char tex_paths_loaded[MAX_TOTAL_TEXS][TEX_PATH_MAX_LEN];
extern struct texture *tex_objs_loaded;

void textures_init(void);
u32 texture_create_file(const char *path);
void texture_destroy(struct texture *t);

#endif /* _ENGINE_TEXTURE_H_ */
