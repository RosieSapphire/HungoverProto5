/**
 * @file src/mesh.c
 */

#include <assert.h>
#include <string.h>
#include <malloc.h>

#include "mesh.h"

u16 mesh_ind_by_name(const char *name, const struct mesh *meshes,
		     const u16 num_meshes)
{
	for (u16 i = 0; i < num_meshes; i++)
	{
		const struct mesh *m = meshes + i;

		if (!strcmp(m->name, name))
			return (i);
	}

	return (0xFFFF);
}
