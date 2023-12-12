#include <malloc.h>
#include <libdragon.h>

#include "engine/vector.h"
#include "engine/collision_mesh.h"

/**
 * collision_mesh_gen - Generates a Collision Mesh from a Normal Mesh
 * @out: Collision Mesh Out
 * @in: Normal Mesh In
 */
void collision_mesh_gen(struct collision_mesh *out,
			 const struct mesh *in)
{
	out->num_verts = in->num_indis;
	out->verts = malloc(sizeof(vec3) * out->num_verts);
	for (u16 i = 0; i < out->num_verts; i++)
	{
		f32 *pi = in->verts[in->indis[i]].pos;
		f32 *po = out->verts[i];

		vector_copy(po, pi, 3);
	}
}
