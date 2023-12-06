#ifndef _ENGINE_MESH_H_
#define _ENGINE_MESH_H_

#include <libdragon.h>

#include "engine/config.h"
#include "engine/types.h"
#include "engine/vertex.h"

enum mesh_flags : u8
{
	MESH_IS_ACTIVE = 0x1,
};

/**
 * struct mesh - Mesh Structure
 * @name: Mesh Name
 * @num_verts: Number of Vertices
 * @num_indis: Number of Indices
 * @verts: Vertices Array
 * @indis: Indices Array
 * @flags: Flags
 * @tind: Texture Index
 */
struct mesh
{
	char name[CONF_NAME_MAX];
	u16 num_verts, num_indis;
	struct vertex *verts;
	u16 *indis;
	rspq_block_t *block;
	u8 flags;
	u16 tind;
};

void mesh_create_data(struct mesh *m, const char *name, const u16 num_verts,
		      const u16 num_indis, const struct vertex *verts,
		      const u16 *indis);
void mesh_destroy(struct mesh *m);
void mesh_draw(const struct mesh *m);
void mesh_gen_rspqblock(struct mesh *m, const u32 tid);
struct mesh *mesh_get_name(struct mesh *array,
			   const char *name, const u16 num_elem);

#endif /* _ENGINE_MESH_H_ */
