#ifndef _ENGINE_COLLISSION_MESH_H_
#define _ENGINE_COLLISSION_MESH_H_

#include "engine/mesh.h"

typedef f32 vec3[3];

enum collision_type
{
	COLTYPE_FLOOR,
	COLTYPE_WALLS,
	COLTYPE_COUNT,
};

struct collision_mesh
{
	u16 num_verts;
	vec3 *verts;
};

void collision_mesh_gen(struct collision_mesh *out,
			 const struct mesh *in);

#endif /* _ENGINE_COLLISSION_MESH_H_ */
